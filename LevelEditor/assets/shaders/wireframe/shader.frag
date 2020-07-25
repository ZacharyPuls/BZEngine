layout(location = SCENE_COLOR_UNIFORM_LOCATION)
uniform vec4 Color;

out vec4 FragColor;

void main()
{
	FragColor = Color;
}
