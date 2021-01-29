#include "cplug_aws.h"
#include "../cplug_singleton.h"
#include "awss3.h"

void Cplug_AWS_InitAwsSdk(const string& sEndPoint, const string& sAccessKeyId, const string& sSecretKey, const string& sRegin)
{
	AwsS3& s3 = Cplug_Singleton<AwsS3>::getInstance();
	s3.init(sEndPoint, sAccessKeyId, sSecretKey, sRegin);
}

Aws::Vector<Bucket> Cplug_AWS_ListBucket()
{
	AwsS3& s3 = Cplug_Singleton<AwsS3>::getInstance();
	return s3.listBucket();
}

Aws::Vector<Object> Cplug_AWS_ListObject(const string& sBucket)
{
	AwsS3& s3 = Cplug_Singleton<AwsS3>::getInstance();
	return s3.listObject(sBucket);
}

bool Cplug_AWS_PutObject(const string& sBucket, const string& sObjectKey, const string& sFileName)
{
	AwsS3& s3 = Cplug_Singleton<AwsS3>::getInstance();
	return s3.putObject(sBucket, sObjectKey, sFileName);
}

bool Cplug_AWS_GetObject(const string& sBucket, const string& sObjectKey, const string& sFileDir, const string& sFileName)
{
	AwsS3& s3 = Cplug_Singleton<AwsS3>::getInstance();
	return s3.getObject(sBucket, sObjectKey, sFileDir, sFileName);
}

bool Cplug_AWS_DelObject(const string& sBucket, const string& sObjectKey)
{
	AwsS3& s3 = Cplug_Singleton<AwsS3>::getInstance();
	return s3.delObject(sBucket, sObjectKey);
}

void Cplug_AWS_DeInitAwsSdk()
{
	AwsS3& s3 = Cplug_Singleton<AwsS3>::getInstance();
	s3.deinit();
}
