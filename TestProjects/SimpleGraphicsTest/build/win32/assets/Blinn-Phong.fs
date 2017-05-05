//
// Structure for material
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
struct MATERIAL
{
    vec4  vAmbient;
    vec4  vDiffuse;
    vec4  vSpecular;
};

//
// Some useful macros
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define saturate(x) clamp( x, 0.0, 1.0 )

//
// Matarial-uniform
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
uniform MATERIAL g_Material;

//
// Varyings
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
varying vec3     g_vViewVecES;
varying vec3     g_vNormalES;
varying vec3     g_vLightVecES;

//
// Shader main
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void main()
{
    // Normalize per-pixel vectors	
    vec3 vNormal = normalize( g_vNormalES );
	vec3 vLight  = normalize( g_vLightVecES );
    vec3 vView   = normalize( g_vViewVecES );
    vec3 vHalf   = normalize( vLight + vView );

	float NdotL = saturate( dot(vNormal,vLight) );
	float NdotH = saturate( dot(vNormal,vHalf) );
	
    // Compute the lighting in eye-space
    float fDiffuse  = NdotL;
    
	// Specular exponent
    float fSpecular = pow( NdotH, g_Material.vSpecular.w );
	float SelfShadow = 4.0 * fDiffuse;

    // Combine lighting with the material properties
    gl_FragColor.rgba  = vec4(0.15,0.15, 0.15, 0.15) * g_Material.vAmbient;
    gl_FragColor.rgba += g_Material.vDiffuse * fDiffuse;
    gl_FragColor.rgb  += SelfShadow * vec3(0.15, 0.15, 0.15) * g_Material.vSpecular.xyz * fSpecular;
    
}

