#pragma once
#include <string>
#include <vector>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/Object.h>

using namespace std;
using namespace Aws::S3;
using namespace Aws::S3::Model;

void Cplug_AWS_InitAwsSdk(const string& sEndPoint, const string& sAccessKeyId, const string& sSecretKey, const string& sRegin = "cn");

Aws::Vector<Bucket> Cplug_AWS_ListBucket();

Aws::Vector<Object> Cplug_AWS_ListObject(const string& sBucket);

bool Cplug_AWS_PutObject(const string& sBucket, const string& sObjectKey, const string& sFileName);

bool Cplug_AWS_GetObject(const string& sBucket, const string& sObjectKey, const string& sFileDir, const string& sFileName = "");

bool Cplug_AWS_DelObject(const string& sBucket, const string& sObjectKey);

void Cplug_AWS_DeInitAwsSdk();
