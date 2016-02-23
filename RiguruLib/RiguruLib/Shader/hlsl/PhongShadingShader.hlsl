matrix g_matW;
matrix g_matV;
matrix g_matP;

/*====頂点シェーダーで使用====**/
float3 g_LightPos;	//ライトの座標
float3 g_EyePos;	//カメラの座標

/*====ピクセルシェーダーで使用====**/
float4 g_AmbientLightColor;		//光の基礎色
float4 g_DiffuseLightColor;		//光の拡散反射色
float4 g_SpecularLightColor;	//光の鏡面反射色

float4 g_MaterialAmbient;		//物体の持つ基礎色
float4 g_MaterialDiffuse;		//物体の持つ拡散反射色
float4 g_MaterialSpecular;		//物体の持つ鏡面反射色

float  g_SpecularPower = 100.0f; //鏡面反射の強度
float  g_SpecularMin = 0.0f;     //鏡面反射範囲値 基本0でOK

// テクスチャー
Texture2D g_Tex;

// サンプラーステート
SamplerState  g_Sampler;

float4 g_vecLight;

float4 materialColor;

float4x4 boneMatrix[256];
float4x4 otherBoneMatrix[256];

float4 blendLevel;
// 頂点シェーダーの入力パラメータ
struct VS_IN
{
	float4 pos   : POSITION;   // 頂点座標
	float3 normal : NORMAL;
	float2 texel : TEXCOORD;   // テクセル
};

// 頂点シェーダーの出力パラメータ
struct VS_OUT
{
	float4 pos    : SV_POSITION;
	float3 normal : NORMAL;
	float2 texel  : TEXCOORD0;
	float3 L	  : TEXCOORD1;	//頂点座標からライト座標への方向
	float3 V	  : TEXCOORD2;	//頂点座標から視点座標への方向
};

//頂点シェーダー＿フォンシェーディング
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out;
	Out = (VS_OUT)0;

	float4 localPos = In.pos;
		float4 W = mul(localPos, g_matW);	//ワールド変換
		float4 WV = mul(W, g_matV);			//ビュー変換
		float4 WVP = mul(WV, g_matP);		//射影変換

		Out.pos = WVP;
	Out.normal = mul(In.normal, (float3x3)g_matW);
	Out.texel = float2(In.texel.x, 1 - In.texel.y);
	Out.L = g_LightPos - W;
	Out.V = g_EyePos - W;

	return Out;
}

//ピクセルシェーダ＿フォンシェーディング
float4 PS_Texture_Phong(VS_OUT In) : SV_TARGET
{
	float3 L = normalize(In.L);
	float3 V = normalize(In.V);
	float3 H = normalize(L + V);
	float3 N = normalize(In.normal);

	float4 baseColor = g_Tex.Sample(g_Sampler, In.texel);
	//拡散反射強度を計算する
	float diffuse = max(dot(N, L), 0.0f);
	//鏡面反射強度を計算する
	float specular = pow(max(dot(N, H), g_SpecularMin), g_SpecularPower);

	float4 totalAmbient = g_MaterialAmbient * g_AmbientLightColor;
		float4 totalDiffuse = g_MaterialDiffuse * g_DiffuseLightColor * diffuse;
		float4 totalSpecular = g_MaterialSpecular * g_SpecularLightColor * specular;

		float4 Out = totalAmbient + totalDiffuse + totalSpecular;
		Out.a = baseColor.a * g_MaterialDiffuse;
	return Out;
}

//ピクセルシェーダ＿フォンシェーディング＆テクスチャ
float4 PS_Texture_PhongTex(VS_OUT In) : SV_TARGET
{
	float3 L = normalize(In.L);
	float3 V = normalize(In.V);
	float3 H = normalize(L + V);
	float3 N = normalize(In.normal);

	float4 baseColor = g_Tex.Sample(g_Sampler, In.texel);
	////拡散反射強度を計算する
	float diffuse = max(dot(N, L), 0.0f);
	////鏡面反射強度を計算する
	float specular = pow(max(dot(N, H), g_SpecularMin), g_SpecularPower);

	float3 totalAmbient = baseColor  * g_MaterialAmbient * g_AmbientLightColor;
	float3 totalDiffuse = baseColor.rgb  * g_MaterialDiffuse * g_DiffuseLightColor * diffuse;
	float3 totalSpecular = g_MaterialSpecular * g_SpecularLightColor * specular;

	float3 totalColor = totalAmbient + totalDiffuse + totalSpecular;
	float4 Out = float4(totalColor, baseColor.a);
	return Out;
}

// エントリーポイントとバージョンを指定する
technique11 TShader
{
	//フォンシェーディング
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Main()));
		SetGeometryShader(NULL);
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Texture_Phong()));
		SetComputeShader(NULL);
	}
	//フォンシェーディング＆テクスチャ
	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Main()));
		SetGeometryShader(NULL);
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Texture_PhongTex()));
		SetComputeShader(NULL);
	}
}