#version 330 core

in vec3  fvertex;
in vec3  fnormal;

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;

uniform vec3 posFocus;
uniform vec3 colFocus;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

out vec4 FragColor;


vec3 Lambert (vec3 NormSCO, vec3 L)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = llumAmbient*fmatamb;

  if (dot (L, NormSCO) > 0)
    colRes += colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = Lambert(NormSCO, L);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{


  vec3 L = normalize(posFocus - fvertex.xyz);
  vec3 NM = normalize(fnormal);
  vec3 color = Phong(NM, L, vec4(fvertex, 1.0));

  FragColor = vec4(color, 1);
}
