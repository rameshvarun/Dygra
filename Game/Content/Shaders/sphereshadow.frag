if (iSphere( {{NAME}} , pos, light, rm, r))
{
	float amb = smoothstep(0.0, 2.0, r);
	co = amb*co;
}