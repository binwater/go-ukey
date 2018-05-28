package ukey

/*
#cgo LDFLAGS: -L ./ -lRockey6SmartPlus
#include "dic32.h"
#include <stdlib.h>
#include <stdio.h>

char *str;
char *chNullName;
void WriteLog(char *str,...){}

// @brief  从UKEY中读取全部数据
// @param[in]       hic				        连接到的UKEY的句柄
// @param[in]       pReadLen				输入读出的数据的长度
// @param[out]      pOutBuf				    读出的数据内容
// @return   成功返回SCARD_S_SUCCESS，失败返回相应的错误码
int  UKEY_ReadAllData(int hic, unsigned short *pReadLen, char *pOutBuf)
{
	int iRetValue = 0;
	unsigned short  iMaxReadLen = 1024 * 2;
	unsigned short  iActualReadLen = 0;
	int i = 0;

	// 输入参数检测
	if(NULL == pReadLen || NULL == pOutBuf || *pReadLen <= 0)
	{
		return 1;
	}

	//一次最多读取数据2K，当读取数据小于2K时，直接调用
	if(*pReadLen < iMaxReadLen)
	{
		//读取全部数据 WORD *
		iRetValue = DIC_ReadFile(hic, 0, pReadLen, pOutBuf);
		if (iRetValue != SCARD_S_SUCCESS)
		{
			return iRetValue;
		}

		return SCARD_S_SUCCESS;
	}

	//当读取数据大于2K时，分段读取
	for(i=0; i < *pReadLen; i=i+iMaxReadLen)
	{
		if(*pReadLen >= i+iMaxReadLen)
		{
			iActualReadLen = iMaxReadLen;
		}
		else
		{
			iActualReadLen = *pReadLen - i;

		}

		if(0 == i)
		{
			//读取数据
			iRetValue = DIC_ReadFile(hic, i, &iActualReadLen, pOutBuf);
			if (iRetValue != SCARD_S_SUCCESS)
			{
				return iRetValue;
			}
		}
		else
		{
			//读取数据
			iRetValue = DIC_ReadFile(hic, i, &iActualReadLen, pOutBuf + i);
			if (iRetValue != SCARD_S_SUCCESS)
			{
				return iRetValue;
			}
		}
	}

	return SCARD_S_SUCCESS;
}
*/
import "C"
import (
	"errors"
	"unsafe"
)

//创建Ukey对象
type Ukey struct {
}

/**
 * @brief	查找Ukey
 * @param[]	无
 * @return   成功返回 查找个数，失败返回error	错误信息
 */
func (ukey *Ukey) FindUkey() (int, error) {
	var gRet int
	cRet := C.int(gRet)

	cRet = C.DIC_Find()
	if cRet <= 0 {
		return int(cRet), errors.New("find failed")
	}

	return int(cRet), nil
}

/**
 * @brief	打开Ukey
 * @param[]	无
 * @return   成功返回 打开锁的句柄，失败返回error	错误信息
 */

func (ukey *Ukey) OpenUkey() (int, error) {
	var gRet int
	cRet := C.int(gRet)

	cRet = C.DIC_Open(0, C.str)
	if cRet < 0 {
		return int(cRet), errors.New("open failed")
	}
	return int(cRet), nil
}

/**
 * @briefi	关闭Ukey
 * @param[in]	hic		OpenUkey返回的句柄
 * @return   成功返回 0，失败返回error	错误信息
 */
func (ukey *Ukey) CloseUkey(hic int) (int, error) {
	var gRet int
	cRet := C.int(gRet)

	cRet = C.DIC_Close(C.int(hic))
	if cRet != 0 {
		return int(cRet), errors.New("Close failed")
	}
	return int(cRet), nil
}

/**
 * @brief	校验Ukey
 * @param[in]	hic		OpenUkey返回的句柄
 * @param[in]	passowrd	ukey 8个字节pin码
 * @return   成功返回 0，失败返回error	错误信息
 */
func (ukey *Ukey) CheckUkey(hic int, passowrd [8]byte) (int, error) {
	if &passowrd == nil {
		return 1, errors.New("input arguments error")
	}

	var gRet int
	cRet := C.int(gRet)
	c_char := (*C.BYTE)(unsafe.Pointer(&passowrd[0]))

	//验证超级秘密
	cRet = C.DIC_CheckSuperPassword(C.int(hic), c_char)
	if cRet != 0 {
		return int(cRet), errors.New("Check failed")
	}

	return int(cRet), nil
}

/**
 * @brief		写Ukey
 * @param[in]	hic		OpenUkey返回的句柄
 * @param[in]	fileName	要写入的文件名字
 * @param[in]	fileSize	要写入数据的大小
 * @param[in]	inData		要写入数据的内容
 * @return   成功返回 0，失败返回error	错误信息
 */
func (ukey *Ukey) WriteUkey(hic int, fileName string, fileSize uint16, inData []byte) (int, error) {
	if len(fileName) == 0 || fileSize == 0 || inData == nil {
		return 1, errors.New("input arguments error")
	}

	var gRet int
	var iFileExistFlag int = 0
	cRet := C.int(gRet)
	cName := C.CString(fileName)

	//查看是否存在文件
	cRet = C.DIC_SetCurrentFile(C.int(hic), 0x3F03, C.chNullName)
	if cRet != 0 {
		iFileExistFlag = 1
	}

	if iFileExistFlag == 0 {
		//删除已经存在文件
		cRet = C.DIC_DeleteFile(C.int(hic))
		if cRet != 0 {
			return int(cRet), errors.New("delete failed")
		}
	}

	cFileSize := C.WORD(fileSize)
	//创建文件
	cRet = C.DIC_CreateFile(C.int(hic), 0x3F03, 0xff, 0x00, cFileSize, 0x00, cName)
	if cRet != 0 {
		return int(cRet), errors.New("create failed")
	}

	cInBuf := (*C.char)(unsafe.Pointer(&inData[0]))
	//写文件
	cRet = C.DIC_WriteFile(C.int(hic), 0, cFileSize, cInBuf)
	if cRet != 0 {
		return int(cRet), errors.New("write failed")
	}

	C.free(unsafe.Pointer(cName)) //释放内存
	return int(cRet), nil
}

/**
 * @brief		读Ukey
 * @param[in]	hic		OpenUkey返回的句柄
 * @param[in]	fileName	要读的文件名字
 * @param[in]	fileSize	要读的数据大小
 * @param[in]	outData		要读的数据内容
 * @return   成功返回 0，失败返回error	错误信息
 */
func (ukey *Ukey) ReadUkey(hic int, fileName string, fileSize *uint16, outData []byte) (int, error) {
	if len(fileName) == 0 || fileSize == nil || outData == nil {
		return 1, errors.New("input arguments error")
	}

	var gRet int
	cRet := C.int(gRet)
	cName := C.CString(fileName)

	//查看是否存在文件
	cRet = C.DIC_SetCurrentFile(C.int(hic), 0x3F03, C.chNullName)
	if cRet != 0 {
		return 1, errors.New("read file input file name not exist")

	}

	cFileSize := (*C.ushort)(unsafe.Pointer(fileSize))
	cOutBuf := (*C.char)(unsafe.Pointer(&outData[0]))

	//读文件
	cRet = C.UKEY_ReadAllData(C.int(hic), cFileSize, cOutBuf)
	if cRet != 0 {
		return int(cRet), errors.New("read failed")
	}

	C.free(unsafe.Pointer(cName)) //释放内存
	return int(cRet), nil
}

/**
 * @brief		获取文件名字和文件大小
 * @param[in]	hic		OpenUkey返回的句柄
 * @param[out]	gFileName	返回文件名字
 * @param[out]	gFileSize	返回文件大小
 * @return   成功返回 0，失败返回error	错误信息
 */
func (ukey *Ukey) GetFileNameAndSize(hic int) (string, uint16, int, error) {
	var gRet int
	cRet := C.int(gRet)

	var gFileId uint16
	cFileId := (C.WORD)(gFileId)

	var gFileType byte
	cFileType := (C.BYTE)(gFileType)

	var gFileSize uint16
	cFileSize := (C.WORD)(gFileSize)

	var gFileAttribute byte
	cFileAttribute := (C.BYTE)(gFileAttribute)

	var gFileSecurity byte
	cFileSecurity := (C.BYTE)(gFileSecurity)

	var gFileName string
	cFileName := C.CString(gFileName)

	//设置当前文件
	cRet = C.DIC_SetCurrentFile(C.int(hic), 0x3F03, C.chNullName)
	if cRet != 0 {
		return "", 0, int(cRet), errors.New("there is no file in ukey.")
	}

	//获取文件名字和大小
	cRet = C.DIC_GetCurrentFile(C.int(hic), &cFileId, &cFileType, &cFileSize, &cFileAttribute, &cFileSecurity, cFileName)
	if cRet != 0 {
		return "", 0, int(cRet), errors.New("get file info failed.")
	}

	return C.GoString(cFileName), uint16(cFileSize), 0, nil
}
