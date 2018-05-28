package main

import (
	"../../ukey"
	"errors"
	"fmt"
	"os"
)

const fileName = "topvsp.conf"

//写
func WriteFile(buf string, name string) error {
	if len(buf) == 0 || len(name) == 0 {
		return errors.New("input arguments error")
	}

	fout, err := os.Create(name)
	defer fout.Close()
	if err != nil {
		return err
	}

	//写入到本地文件中
	fout.WriteString(buf)

	return nil
}

//读
func ReadFile(name string) ([]byte, error) {
	if len(name) == 0 {
		return nil, errors.New("input arguments error")
	}

	//打开本地文件 读取出全部数据
	fin, err := os.Open(name)
	defer fin.Close()
	if err != nil {
		return nil, errors.New("Close error")
	}

	buf_len, _ := fin.Seek(0, os.SEEK_END)
	fin.Seek(0, os.SEEK_SET)

	buf := make([]byte, buf_len)
	fin.Read(buf)

	return buf, nil
}

func main() {

	var ukey ukey.Ukey

	//找UKEY
	gHic, err := ukey.FindUkey()
	if gHic <= 0 {
		fmt.Println(err)
		return
	}
	fmt.Println("Find successful\n")

	//打开UKEY
	gHic, err = ukey.OpenUkey()
	if gHic < 0 {
		fmt.Println(err)
		return
	}
	fmt.Println("Open successful\n")
	/*
		var pass = [8]byte{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
		//校验UKEY
		gRet, err := ukey.CheckUkey(gHic, pass)
		if gRet != 0 {
			fmt.Println(err)
			return
		}
		fmt.Println("Check successful\n")
	*/
	/*
		//获取UKEY文件名和大小
		fileName, fileSize, _, err := ukey.GetFileNameAndSize(gHic)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println("get file name size successful\n")
		fmt.Println(fileName, fileSize)
		/*
			wBuf := []byte("test helloworld.")
			//写入UKEY
			gRet, err := ukey.WriteUkey(gHic, fileName, 16, wBuf)
			if gRet != 0 {
				fmt.Println(err)
				return
			}
			fmt.Println("Write successful\n")
	*/ /*
		var size uint16 = fileSize
		rBuf := make([]byte, fileSize)
		fmt.Println("len(rBuf) is ", len(rBuf))
		//读取UKEY
		gRet, err := ukey.ReadUkey(gHic, fileName, &size, rBuf)
		if gRet != 0 {
			fmt.Println(err)
			return
		}
		fmt.Println("Read successful\n")
		//	fmt.Println("data is", string(rBuf[:]))
		//	fmt.Println("len data is", len(string(rBuf[:])))

		// 写入指定文件
		err = WriteFile(string(rBuf[:]), "ukeyfile.txt")
		if err != nil {
			return
		}
	*/

	wBuf, _ := ReadFile("ukeyfile.txt")

	size := (uint16)(len(wBuf))
	fmt.Println("file size is ", size)

	//写入UKEY
	gRet, err := ukey.WriteUkey(gHic, "ukeyfile.txt", size, wBuf)
	if gRet != 0 {
		fmt.Println(err)
		return
	}
	fmt.Println("Write successful\n")

	//关闭锁
	gRet, err = ukey.CloseUkey(gHic)
	if gRet != 0 {
		fmt.Println(err)
		return
	}
	fmt.Println("Close successful\n")
}
