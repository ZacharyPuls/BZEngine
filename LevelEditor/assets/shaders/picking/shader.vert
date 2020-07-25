layout(location = PICKING_POSITION_ATTRIB_LOCATION)
in vec3 Position;

layout(location = PICKING_MVP_UNIFORM_LOCATION)
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(Position, 1.0f);
}
