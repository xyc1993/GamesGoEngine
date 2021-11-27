#include "SharedAsset.h"

SharedAsset::SharedAsset()
{
	numberOfUsers = 0;
}

void SharedAsset::DeleteSafely()
{
	if (numberOfUsers == 0)
	{
		delete this;
	}
	else
	{
		DecrementNumberOfUsers();
	}
}

unsigned int SharedAsset::GetNumberOfUsers() const
{
	return numberOfUsers;
}

void SharedAsset::IncrementNumberOfUsers()
{
	numberOfUsers++;
}

void SharedAsset::DecrementNumberOfUsers()
{
	if (numberOfUsers > 0)
	{
		numberOfUsers--;
	}
}