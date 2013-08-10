level = Level( "raytracing" )
level:LoadXML("level1.xml")

level:BuildShader()
level:run()