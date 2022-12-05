#include "Rendering.h"
#include "../../Platform/Windows/WindowsEngine.h"

vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;

IRenderingInterface::IRenderingInterface()
{
	create_guid(&Guid);
	RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	for (vector<IRenderingInterface*>::const_iterator Iter = RenderingInterface.begin(); Iter != RenderingInterface.end();++Iter)
	{
		if (*Iter == this)
		{
			RenderingInterface.erase(Iter);
			break;
		}
	}
}

void IRenderingInterface::Init()
{

}

void IRenderingInterface::PreDraw(float DeltaTime)
{
	//重值我们的命令列表
	ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), NULL));
}

void IRenderingInterface::Draw(float DeltaTime)
{

}

void IRenderingInterface::PostDraw(float DeltaTime)
{

}

ComPtr<ID3D12Resource> IRenderingInterface::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& uploadBuffer, const void* InData, UINT64 InDataSize)
{
	//创建实际的默认缓冲区资源
	ComPtr<ID3D12Resource> DefaultBuffer;
	CD3DX12_RESOURCE_DESC BufferResourceDESC = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDESC,
		D3D12_RESOURCE_STATE_COMMON,
		NULL,IID_PPV_ARGS(DefaultBuffer.GetAddressOf())));

	//为了将CPU端内存中的数据复制到默认缓冲区，我们还需要创建一个处于中介位置的上传堆
	CD3DX12_HEAP_PROPERTIES UpdateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&UpdateBufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL,IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

	//描述我们希望复制到默认缓冲区的数据
	D3D12_SUBRESOURCE_DATA SubResourceData = { };
	SubResourceData.pData = InData;
	SubResourceData.RowPitch = InDataSize;
	SubResourceData.SlicePitch = SubResourceData.RowPitch;

	//将数据复制到默认缓冲区资源的流程
	//UpdateSubresources 辅助函数先将数据从CPU端的内存中复制到位于中介位置的上传堆里，
	//接着再通过ID3D12CommandList::UpdateSubresources函数,把上传堆内的数据复制到mBuffer中
	CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_DEST);
	GetGraphicsCommandList()->ResourceBarrier(1,&CopyDestBarrier);

	UpdateSubresources<1>(
		GetGraphicsCommandList().Get(),
		DefaultBuffer.Get(),
		uploadBuffer.Get(),
		0,//0 -> D3D12_REQ_SUBRESOURCES
		0,//0 -> D3D12_REQ_SUBRESOURCES
		1,
		&SubResourceData);

	CD3DX12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);
	//GetGraphicsCommandList()->ResourceBarrier(1,&ReadDestBarrier);
	//注意：在调用上述函数后，必须保证uploadBuffer依然存在，而不能对它立即进行销毁。
	//这是因为命令列表中的复制操作可能尚未执行。待调用者得知复制完成的消息后，方可释放uploadBuffer

	return DefaultBuffer;

}

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		return InEngine->D3dDevice;
	}
	return NULL;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetGraphicsCommandList()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		return InEngine->GraphicsCommandList;
	}
	return NULL;
}

ComPtr<ID3D12CommandAllocator> IRenderingInterface::GetCommandAllocator()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->CommandAllocator;
	}

	return NULL;
}

#if defined(_WIN32)
CWindowsEngine* IRenderingInterface::GetEngine()
{
	return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
CEngine* IRenderingInterface::GetEngine()
{
	return Engine;
}
#endif


FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{

}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer)
	{
		UploadBuffer->Unmap(0,NULL);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElemetSize,UINT InElemetCount)
{
	assert(InDevice);

	ElementSize = InElemetSize;
	CD3DX12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InElemetSize* InElemetCount);
	ANALYSIS_HRESULT(InDevice->CreateCommittedResource(
		&HeapPropertie,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&UploadBuffer)));

	ANALYSIS_HRESULT(UploadBuffer->Map(0,nullptr,reinterpret_cast<void**>(&Data)));
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	memcpy(&Data[Index* ElementSize],InData,ElementSize);
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT InTypeSize)
{
	//方法一(InTypeSize + 255)& ~255;
	/*方法二
	if (!(InTypeSize % 256))
	{
		float NewFloat = (float)InTypeSize/256;
		int Num = (NewFloat += 1);
		InTypeSize = Num * 256;
	}*/
	return (InTypeSize + 255)& ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(ElementSize);
}
