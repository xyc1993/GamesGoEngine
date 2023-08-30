#version 420 core

out int objectId;

uniform int objectIdNumber = -1;

void main()
{
	objectId = objectIdNumber;
}