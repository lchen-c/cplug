#pragma once
#include <aws/s3/S3Client.h>
#include <aws/core/Aws.h>
#include <string>
#include <aws/s3/model/Bucket.h>

using namespace std;
using namespace Aws::S3;
using namespace Aws::S3::Model;

class AwsS3
{
public:
	AwsS3();
	~AwsS3();
	void init(const string& sEndPoint, const string& sAccessKeyId, const string& sSecretKey, const string& sRegin = "cn");
	void deinit();
	Aws::Vector<Bucket> listBucket();
	Aws::Vector<Object> listObject(const string& sBucket);
	bool putObject(const string& sBucket, const string& sObjectKey, const string& sFileName);
	bool getObject(const string& sBucket, const string& sObjectKey, const string& sFileDir, const string& sFileName = "");
	bool delObject(const string& sBucket, const string& sObjectKey);
	Aws::S3::S3Client* m_Client;
	Aws::SDKOptions m_Options;
};