if( E.x > {{NAME}}_min.x && E.y > {{NAME}}_min.y && E.z > {{NAME}}_min.z)
{
	if( E.x < {{NAME}}_max.x && E.y < {{NAME}}_max.y && E.z < {{NAME}}_max.z)
	{
		rayDir = normalize( rayDir * {{NAME}}_scale );
	}
}