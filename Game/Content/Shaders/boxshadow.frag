if (iBox( {{NAME}}_min, {{NAME}}_max, pos, light, rm, r))
{
	float amb = smoothstep(0.0, 2.0, r);
	co = amb*co;
}