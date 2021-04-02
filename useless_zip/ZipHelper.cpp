#include "stdafx.h"
#include "ZipHelper.h"

ZipHelper::ZipHelper()
{
	m_nZipCnt = 0;
}

ZipHelper::~ZipHelper()
{
}

BOOL ZipHelper::Zip_PackFiles(vector<CString> arr, CString mZipFileFullPath)
{
	for (int i = 0; i < arr.size(); ++i)
	{
		//�������� 
		if ((arr[i] == "") || (mZipFileFullPath == ""))
		{
			//·���쳣���� 
			return FALSE;
		}

		if (!IsFolderExist(arr[i]) && !IsFileExist(arr[i]))
		{
			//Ҫ��ѹ�����ļ��в����� 
			return FALSE;
		}

		CString tZipFilePath = mZipFileFullPath.Left(mZipFileFullPath.ReverseFind('\\') + 1);
		if (!IsFolderExist(tZipFilePath))
		{
			//ZIP�ļ���ŵ��ļ��в����ڴ����� 
			wchar_t* temp = (wchar_t*)tZipFilePath.GetBuffer(tZipFilePath.GetLength());
			if (FALSE == CreatedMultipleDirectory(temp))
			{
				//����Ŀ¼ʧ�� 
				return FALSE;
			}
		}

		//����ļ��е����� 
		if (arr[i].Right(1) == '\\')
		{
			this->m_FolderPath = arr[i].Left(arr[i].GetLength() - 1);
			m_FolderName = m_FolderPath.Right(m_FolderPath.GetLength() - m_FolderPath.ReverseFind('\\') - 1);
		}
		else
		{
			this->m_FolderPath = arr[i];
			m_FolderName = arr[i].Right(arr[i].GetLength() - arr[i].ReverseFind('\\') - 1);
		}

		/************************************************************************/

		//����ZIP�ļ� 
		if (m_nZipCnt == 0)
		{
			m_hz = CreateZip(mZipFileFullPath, 0);
		}

		if (m_hz == 0)
		{
			//����Zip�ļ�ʧ�� 
			return FALSE;
		}

		//�ݹ��ļ���,����ȡ���ļ�����ZIP�ļ� 
		m_nZipCnt++;
		BrowseFile(arr[i]);
		//�ر�ZIP�ļ� 
		if (m_nZipCnt == m_arrFileFullName.size())
		{
			CloseZip(m_hz);
			m_nZipCnt = 0;
			break;
		}
	}

	/************************************************************************/

	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//ѹ��ʧ��(δ����ѹ������ļ�) 
		return FALSE;
	}

	return TRUE;
}

/
// ����˵��: ��ѹ���ļ��� 
// ����˵��: [in]�� mUnPackPath     ��ѹ���ļ���ŵ�·�� 
//                         mZipFileFullPath  ZIP�ļ���·��
/
BOOL ZipHelper::Zip_UnPackFiles(CString mZipFileFullPath, CString mUnPackPath)
{
	//�������� 
	if ((mUnPackPath == "") || (mZipFileFullPath == ""))
	{
		//·���쳣���� 
		return FALSE;
	}

	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//ѹ��ʧ��(δ����ѹ���ļ�) 
		return FALSE;
	}

	//�����ѹ����·�������� ��ͼ������ 
	CString tZipFilePath = mUnPackPath;
	if (!IsFolderExist(tZipFilePath))
	{
		//��ѹ���ŵ��ļ��в����� ������ 
		wchar_t* temp = (wchar_t*)tZipFilePath.GetBuffer(tZipFilePath.GetLength());
		if (FALSE == CreatedMultipleDirectory(temp))
		{
			//����Ŀ¼ʧ�� 
			return FALSE;
		}
	}
	/************************************************************************/
	//��ZIP�ļ� 
	m_hz = OpenZip(mZipFileFullPath, 0);
	if (m_hz == 0)
	{
		//��Zip�ļ�ʧ�� 
		return FALSE;
	}

	m_zr = SetUnzipBaseDir(m_hz, mUnPackPath);
	if (m_zr != ZR_OK)
	{
		//��Zip�ļ�ʧ�� 
		CloseZip(m_hz);
		return FALSE;
	}

	m_zr = GetZipItem(m_hz, -1, &m_ze);
	if (m_zr != ZR_OK)
	{
		//��ȡZip�ļ�����ʧ�� 
		CloseZip(m_hz);
		return FALSE;
	}

	int numitems = m_ze.index;
	for (int i = 0; i < numitems; i++)
	{
		m_zr = GetZipItem(m_hz, i, &m_ze);
		m_zr = UnzipItem(m_hz, i, m_ze.name);

		if (i == 100)
		{
			int x = 1;
		}
		if (m_zr != ZR_OK)
		{
			//��ȡZip�ļ�����ʧ�� 
			CloseZip(m_hz);
			return FALSE;
		}
	}

	m_nZipCnt = 0;
	CloseZip(m_hz);
	return TRUE;
}

/
// ����˵��: �����ļ��� 
// ����˵��: [in]��strFile �������ļ���(�˷�����������·��ĩβ���*.*) 
// ����ֵ:BOOL����,���ڷ���TRUE,����ΪFALSE 
/
void ZipHelper::BrowseFile(CString & strFile)
{
	CFileFind ff;
	CString szDir = strFile;

	WIN32_FIND_DATAA FindFileData;
	FindFirstFile(strFile, &FindFileData);
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//Folder
	{
		if (szDir.Right(1) != "\\")
			szDir += "\\";
		szDir += "*.*";
	}
	else//File
	{
		int xx = 21;
	}

	BOOL res = ff.FindFile(szDir);
	while (res)
	{
		res = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots())
		{
			//�����һ����Ŀ¼���õݹ��������һ���� 
			CString strPath = ff.GetFilePath();

			CString subPath;
			GetRelativePath(strPath, subPath);
			//���ļ���ӵ�ZIP�ļ� 
			ZipAddFolder(m_hz, subPath);
			BrowseFile(strPath);
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			//��ʾ��ǰ���ʵ��ļ�(����·��) 
			CString strPath = ff.GetFilePath();
			CString subPath;

			GetRelativePath(strPath, subPath);
			//���ļ���ӵ�ZIP�ļ� 
			ZipAdd(m_hz, subPath, strPath);
		}
	}
	//�ر� 
	ff.Close();
}

/
// ����˵��: ��ȡ���·�� 
// ����˵��: [in]��pFullPath ��ǰ�ļ�������·�� [out] : ����������·�� 
/
void ZipHelper::GetRelativePath(CString & pFullPath, CString & pSubString)
{
	pSubString = pFullPath.Right(pFullPath.GetLength() - GetAppPath().GetLength());
}

/
// ����˵��: �����༶Ŀ¼ 
// ����˵��: [in]�� ·���ַ��� 
// ����ֵ: BOOL �ɹ�True ʧ��False 
/
BOOL ZipHelper::CreatedMultipleDirectory(wchar_t* direct)
{
	std::wstring Directoryname = direct;

	if (Directoryname[Directoryname.length() - 1] != '\\')
	{
		Directoryname.append(1, '\\');
	}
	std::vector< std::wstring> vpath;
	std::wstring strtemp;
	BOOL  bSuccess = FALSE;
	for (int i = 0; i < Directoryname.length(); i++)
	{
		if (Directoryname[i] != '\\')
		{
			strtemp.append(1, Directoryname[i]);
		}
		else
		{
			vpath.push_back(strtemp);
			strtemp.append(1, '\\');
		}
	}
	vector< std::wstring>::const_iterator vIter = vpath.begin();
	for (vIter; vIter != vpath.end(); vIter++)
	{
		bSuccess = CreateDirectory((LPCTSTR)vIter->c_str(), NULL) ? TRUE : FALSE;
	}

	return bSuccess;
}

/
// ����˵��: ���ָ�����ļ����Ƿ���� 
// ����˵��: [in]��strPath �����ļ��� (�˷�����������·��ĩβ���*.*) 
// ����ֵ:BOOL����,���ڷ���TRUE,����ΪFALSE 
/
BOOL ZipHelper::IsFolderExist(CString & strPath)
{
	CString sCheckPath = strPath;

	if (sCheckPath.Right(1) != '\\')
		sCheckPath += '\\';

	sCheckPath += "*.*";

	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(sCheckPath, &wfd);

	if ((hFind != INVALID_HANDLE_VALUE) &&
		(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))
	{
		//������ڲ��������ļ��� 
		rValue = TRUE;
	}

	FindClose(hFind);
	return rValue;
}
BOOL ZipHelper::IsFileExist(CString& strPath)
{
	DWORD dwAttrib = GetFileAttributes(strPath);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}
void ZipHelper::PressFolder(CString strTgt, CString strSrcFolder, vector<CString> arrSrcFloder)
{
	m_arrFileFullName.clear();

	if (!strSrcFolder.IsEmpty())
	{
		m_arrFileFullName.push_back(strSrcFolder);
	}
	if (!arrSrcFloder.empty())
	{
		for (auto it : arrSrcFloder)
		{
			m_arrFileFullName.push_back(it);
		}
	}

	//Ŀ���ļ���
	string patm_hzipFile = strTgt;
	Zip_PackFiles(m_arrFileFullName, patm_hzipFile.c_str());
}


void ZipHelper::UnPressFolder(CString strTgt)
{
	if (strTgt.IsEmpty())
		return;

	string patm_hzipFile = strTgt;
	string mUnPackPath = GetAppPath();

	Zip_UnPackFiles(patm_hzipFile.c_str(), mUnPackPath.c_str());
}