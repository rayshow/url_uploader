#ifndef __URL_UPLOADER_NONCOPYABLE_H
#define __URL_UPLOADER_NONCOPYABLE_H

namespace url_upload
{
	class noncopyable
	{
		noncopyable()=default;
		noncopyable(const noncopyable& ) = delete;
		noncopyable& operator=(noncopyable const& ) = delete;
	};

}
#endif