
#include"upload.h"
#include"url_uploader.h"

url_upload::url_uploader* uploader = nullptr;

bool url_upload_init()
{
	try
	{
		uploader = new url_upload::url_uploader{};
	}
	catch (...)
	{
		if (uploader)
			delete uploader;
		return false;
	}
	return true;
}

void url_upload_post(const char* url, const char* data)
{
	if(uploader)
		uploader->post(url, data);
}

void url_upload_cleanup()
{
	if (uploader)
	{
		delete uploader;
		uploader = nullptr;
	}
}