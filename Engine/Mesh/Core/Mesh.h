#pragma once

#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;

	static XMFLOAT4X4 IdentityMatrix4x4();
};

class CMesh :public CCoreMinimalObject , public IRenderingInterface 
{
public:
	CMesh();

	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData *InRenderingData);

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	static CMesh* CreateMesh(const FMeshRenderingData *InRenderingData);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;
	ComPtr<ID3DBlob> CPUIndexBufferPtr;

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	ComPtr<ID3D12RootSignature>  RootSignature;
	ComPtr<ID3D12DescriptorHeap> CBVHeap;
	shared_ptr<FRenderingResourcesUpdate> objectConstants;

	ComPtr<ID3D12PipelineState> PSO;

	FShader VertexShader;
	FShader PixelShader;
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;
protected:
	UINT VertexSizeInBytes;
	UINT VertexStrideInBytes;

	UINT IndexSizeInBytes;
	DXGI_FORMAT IndexFormat;
	UINT IndexSize;

	XMFLOAT4X4 WorldMatrix;
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectMatrix;
};