#pragma once
#pragma execution_character_set("utf-8")

#include <WinSock2.h>
#include <IPHlpApi.h>
#include <iostream>
#include <string>
#include <fstream>

#pragma comment(lib,"IPHLPAPI.lib")

using namespace std;

//GetAdaptersInfo
bool GetMacByGetAdaptersInfo(string &macOUT)
{
	bool ret = false;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
	{
		return false;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL)
			return false;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
	{
		for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			if (pAdapter->Type != MIB_IF_TYPE_ETHERNET)
			{
				continue;
			}
			if (pAdapter->AddressLength != 6)
			{
				continue;
			}
			char acMac[32];
			sprintf(acMac, "%02X-%02X-%02X-%02X-%02X-%02X",
				int(pAdapter->Address[0]),
				int(pAdapter->Address[1]),
				int(pAdapter->Address[2]),
				int(pAdapter->Address[3]),
				int(pAdapter->Address[4]),
				int(pAdapter->Address[5])
				);
			ret = true;
			macOUT = acMac;
			break;
		}
	}

	free(pAdapterInfo);

	return ret;
}

int main(int argc, char * argt)
{
	string acmac;
	bool ret = true;

	ret = GetMacByGetAdaptersInfo(acmac);

	if (ret == true)
	{
		ofstream out("lisence");
		if (out.is_open())
		{
			for (auto &r : acmac)
			{
				out << r;
			}
			out.close();
			cout << "generate lisence file succeed!" << endl;
			cout << "The MAC addres is: ";
			for (auto &r : acmac)
			{
				cout << r;
			}
			cout << endl;
			system("pause");
		}
		else
		{
			cout << "open file failed." << endl;
			system("pause");
		}
			
	}
	else
	{
		cout << "failed" << endl;
		system("pause");
	}

	return 0;
}
