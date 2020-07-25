layout(location = PICKING_INSTANCEID_UNIFORM_LOCATION)
uniform uint InstanceID;

layout(location = PICKING_COLOR_ATTACHMENT)
out uint FragColor;

void main()
{
	FragColor = InstanceID;
}
