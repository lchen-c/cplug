#include "awss3.h"
#include <aws/s3/S3Client.h>
#include <aws/core/Aws.h>
#include "../log/cplug_log.h"
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/Object.h>
#include <aws/core/http/Scheme.h>
#include <iostream>
#include <fstream>
#include "../cplug_filedir.h"

namespace Aws
{
	namespace Http
	{

		const char DATE_HEADER[] = "date";
		const char AWS_DATE_HEADER[] = "X-Amz-Date";
		const char AWS_SECURITY_TOKEN[] = "X-Amz-Security-Token";
		const char ACCEPT_HEADER[] = "accept";
		const char ACCEPT_CHAR_SET_HEADER[] = "accept-charset";
		const char ACCEPT_ENCODING_HEADER[] = "accept-encoding";
		const char AUTHORIZATION_HEADER[] = "authorization";
		const char AWS_AUTHORIZATION_HEADER[] = "authorization";
		const char COOKIE_HEADER[] = "cookie";
		const char CONTENT_LENGTH_HEADER[] = "content-length";
		const char CONTENT_TYPE_HEADER[] = "content-type";
		const char TRANSFER_ENCODING_HEADER[] = "transfer-encoding";
		const char USER_AGENT_HEADER[] = "user-agent";
		const char VIA_HEADER[] = "via";
		const char HOST_HEADER[] = "host";
		const char AMZ_TARGET_HEADER[] = "x-amz-target";
		const char X_AMZ_EXPIRES_HEADER[] = "X-Amz-Expires";
		const char CONTENT_MD5_HEADER[] = "content-md5";
		const char API_VERSION_HEADER[] = "x-amz-api-version";
		const char SDK_INVOCATION_ID_HEADER[] = "amz-sdk-invocation-id";
		const char SDK_REQUEST_HEADER[] = "amz-sdk-request";
		const char CHUNKED_VALUE[] = "chunked";

	} // Http
} // Aws


AwsS3::AwsS3()
{
}

AwsS3::~AwsS3()
{
}

void AwsS3::init(const string& sEndPoint, const string& sAccessKeyId, const string& sSecretKey, const string& sRegin)
{
	Aws::InitAPI(m_Options);
	Aws::Client::ClientConfiguration cfg;
	cfg.endpointOverride = sEndPoint.c_str();
	cfg.region = sRegin.c_str();
	cfg.scheme = Aws::Http::Scheme::HTTP;
	cfg.verifySSL = false;
	Aws::Auth::AWSCredentials cred(sAccessKeyId.c_str(), sSecretKey.c_str());
	m_Client = new Aws::S3::S3Client(cred, cfg, Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Always, false);
}

void AwsS3::deinit()
{
	Aws::ShutdownAPI(m_Options);
}

Aws::Vector<Bucket> AwsS3::listBucket()
{
	auto response = m_Client->ListBuckets();
	if (response.IsSuccess()) 
	{
		auto buckets = response.GetResult().GetBuckets();
		return buckets;
	}
	else 
	{
		std::cout << "Error while ListBuckets " << response.GetError().GetExceptionName() << ":" << response.GetError().GetMessage() << std::endl;
	}
	return Aws::Vector<Bucket>();
}

Aws::Vector<Object> AwsS3::listObject(const string& sBucket)
{
	Aws::S3::Model::ListObjectsRequest objects_request;
	objects_request.WithBucket(sBucket.c_str());

	auto list_objects_outcome = m_Client->ListObjects(objects_request);

	if (list_objects_outcome.IsSuccess())
	{
		Aws::Vector<Aws::S3::Model::Object> object_list = list_objects_outcome.GetResult().GetContents();

		return object_list;
	}
	else
	{
		std::cout << "Error while ListObjects " << sBucket.c_str() << ":" << list_objects_outcome.GetError().GetExceptionName() << "[bucket:" << list_objects_outcome.GetError().GetMessage() << "]" << std::endl;
	}
	return Aws::Vector<Object>();
}

bool AwsS3::putObject(const string& sBucket, const string& sObjectKey, const string& sFileName)
{
	Aws::S3::Model::PutObjectRequest object_request;
	object_request.SetBucket(sBucket.c_str());
	object_request.SetKey(sObjectKey.c_str());
	const shared_ptr<Aws::IOStream> input_data = Aws::MakeShared<Aws::FStream>("SampleAllocationTag", sFileName.c_str(), ios_base::in | ios_base::binary);
	object_request.SetBody(input_data);
	auto put_object_outcome = m_Client->PutObject(object_request);
	if (!put_object_outcome.IsSuccess()) 
	{
		auto error = put_object_outcome.GetError();
		std::cout << error.GetExceptionName() << ":" << error.GetMessage()  << std::endl;
		return false;
	}
	return true;
}

bool AwsS3::getObject(const string& sBucket, const string& sObjectKey, const string& sFileDir, const string& sFileName)
{
	Aws::S3::Model::GetObjectRequest get_object_request;
	get_object_request.WithBucket(sBucket.c_str()).WithKey(sObjectKey.c_str());
	Aws::S3::Model::GetObjectOutcome get_object_outcome = m_Client->GetObject(get_object_request);
	if (get_object_outcome.IsSuccess())
	{
		Aws::OFStream local_file;
		string sFullFileName;
		if (sFileName.empty())
		{
			sFullFileName = Cplug_FD_SpliceDirAndFileName(sFileDir, sObjectKey);
		}
		else
		{
			sFullFileName = Cplug_FD_SpliceDirAndFileName(sFileDir, sFileName);
		}
		
		local_file.open(sFullFileName.c_str(), std::ios::out | std::ios::binary);
		local_file << get_object_outcome.GetResult().GetBody().rdbuf();
		local_file.close();
		return true;
	}
	else
	{
		std::cout << "GetObject error: " << get_object_outcome.GetError().GetExceptionName() << ":" << get_object_outcome.GetError().GetMessage() << std::endl;
	}
	return false;
}

bool AwsS3::delObject(const string& sBucket, const string& sObjectKey)
{
	Aws::S3::Model::DeleteObjectRequest request;
	request.WithKey(sObjectKey.c_str()).WithBucket(sBucket.c_str());
	Aws::S3::Model::DeleteObjectOutcome outcome = m_Client->DeleteObject(request);
	if (!outcome.IsSuccess())
	{
		auto err = outcome.GetError();
		std::cout << "Error: DeleteObject [" << err.GetExceptionName() << ":"<< err.GetMessage() <<"]" << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}
