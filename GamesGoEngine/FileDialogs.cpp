#include "FileDialogs.h"

#include "WindowManager.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace GamesGoEngine
{
	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA openFile;
		CHAR fileBuffer[260] = { 0 };
		ZeroMemory(&openFile, sizeof(OPENFILENAME));
		openFile.lStructSize = sizeof(OPENFILENAME);
		openFile.hwndOwner = glfwGetWin32Window((GLFWwindow*)WindowManager::GetWindow());
		openFile.lpstrFile = fileBuffer;
		openFile.nMaxFile = sizeof(fileBuffer);
		openFile.lpstrFilter = filter;
		openFile.nFilterIndex = 1;
		openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&openFile) == TRUE)
		{
			return openFile.lpstrFile;
		}
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA openFile;
		CHAR fileBuffer[260] = { 0 };
		ZeroMemory(&openFile, sizeof(OPENFILENAME));
		openFile.lStructSize = sizeof(OPENFILENAME);
		openFile.hwndOwner = glfwGetWin32Window((GLFWwindow*)WindowManager::GetWindow());
		openFile.lpstrFile = fileBuffer;
		openFile.nMaxFile = sizeof(fileBuffer);
		openFile.lpstrFilter = filter;
		openFile.nFilterIndex = 1;
		openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&openFile) == TRUE)
		{
			return openFile.lpstrFile;
		}
		return std::string();
	}
}