#pragma once

/*
 * Class responsible for how many objects are referencing the 'SharedClass' object
 * Important when calling destructors because for example Meshes or Materials can be used by several MeshRenderers
 * Because of that MeshRenderer needs to know if it can safely delete used material/mesh or not
 */
class SharedAsset
{
public:
	SharedAsset();
	void DeleteSafely();

	unsigned int GetNumberOfUsers() const;
	void IncrementNumberOfUsers();
	void DecrementNumberOfUsers();

private:
	// how many Objects are using/referencing this object, important in determining if object should be destroyed or not
	unsigned int numberOfUsers;
};