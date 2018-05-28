// go run ukey-gen.go -input="somestring"
package main

import (
	"errors"
	"flag"
	"fmt"
	"io/ioutil"

	"git.cloud.top/zhang_xianbin/ukey"
)

const DefaultFileName string = "topvsp.conf"

type Args struct {
	Input string
}

func (a *Args) Parse() {
	input := flag.String("input", "", "license datafile path")
	flag.Parse()
	a.Input = *input
}

func UKeyRead() ([]byte, error) {
	var ukey ukey.Ukey
	gHic, err := ukey.FindUkey()
	if err != nil || gHic <= 0 {
		return nil, errors.New("find ukey failed")
	}
	gHic, err = ukey.OpenUkey()

	defer func(int) {
		//关闭锁
		r, err := ukey.CloseUkey(gHic)
		if err != nil || r != 0 {
			fmt.Println("Close key failed")
		}
	}(gHic)

	if err != nil || gHic < 0 {
		return nil, errors.New("open ukey failed")
	}

	filename, filesize, code, err := ukey.GetFileNameAndSize(gHic)
	if code != 0 || err != nil {
		return nil, errors.New("get filename/size failed")
	}

	rBuf := make([]byte, filesize)
	//读取UKEY
	gRet, err := ukey.ReadUkey(gHic, filename, &filesize, rBuf)
	if err != nil || gRet != 0 {
		return nil, errors.New("read ukey failed")
	}
	return rBuf, nil

}

func UKeyWrite(data []byte) error {
	var ukey ukey.Ukey
	gHic, err := ukey.FindUkey()
	if err != nil || gHic <= 0 {
		return errors.New("find ukey failed")
	}
	gHic, err = ukey.OpenUkey()

	defer func(int) {
		r, err := ukey.CloseUkey(gHic)
		if err != nil || r != 0 {
			fmt.Println("Close key failed")
		}
	}(gHic)

	if err != nil || gHic < 0 {
		return errors.New("open ukey failed")
	}

	filename, _, code, err := ukey.GetFileNameAndSize(gHic)
	if code != 0 || err != nil {
		filename = DefaultFileName
		// return errors.New("get filename/size failed")
	}

	r, err := ukey.WriteUkey(gHic, filename, uint16(len(data)), data)
	if err != nil || r != 0 {
		return errors.New("ukey write failed")
	}
	return nil
}

func main() {
	var args Args
	args.Parse()

	data, err := ioutil.ReadFile(args.Input)
	if err != nil {
		fmt.Println("read data file failed:", err)
		return
	}

	err = UKeyWrite(data)
	if err != nil {
		fmt.Println("write failed:", err)
		return
	} else {
		fmt.Println("write succeed")
	}
	r, err := UKeyRead()
	if err != nil {
		fmt.Println("read failed:", err)
		return
	} else {
		fmt.Println("read succeed")
	}
	if string(r) != string(data) {
		fmt.Println("verify failed:", err)
		return
	} else {
		fmt.Println("verify succeed")
	}
}
