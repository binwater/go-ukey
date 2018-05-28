#ifndef __DIC32_H__
#define __DIC32_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BYTE
typedef	 unsigned char   BYTE;
#endif

#ifndef WORD
typedef	 unsigned short  WORD;
#endif

//#ifndef	DWORD
typedef  unsigned int    DWORD;
//#endif

#ifndef	LONG
typedef  long	LONG;
#endif

#ifndef	BOOL
typedef  short	BOOL;
#endif
// ====================================================================

// 1. Search for dongle device(s) connected to the computer
int DIC_Find();
// Return value:
// it is < 0, indicating an error code is returned, for details, please see below.
// If it is = 0, no card reader device exists
// If it is >0, the return value is the number of card reader device(s) 

//(Note: Normally, even if the user does not connect any device, the driver program will report the card presence 
//due to the existence of the driver. It is necessary to confirm the real device presence with command DIC_Open.) 
//Suppose that maximum 32 card reader devices can be connected to a computer. 

// ====================================================================

// 2. Open the specified card reader
int DIC_Open(int hic, char* reader_name);
// Input:
// hic	Card handle,it's a integer value.
// (1)positive value, it is a enumeration value£¬any one of 0, 1, 2, ... return value of DIC_Find(). If reader_name 
//     is not null, card reader name is returned.
// (2)negative value£¬reader_name parameter can not be null£¬DIC_Open will attempt to open the card reader directly 
//     by the device name in reader_name; in case of a virtual device, it is the path directory of the file to be opened. 

// reader_name  name of card reader.
// An input/output parameter, see comments on hic

// Return value:
// >=0				Success, the opened handle is returned
// <0				An error code is returned, for detailed descriptions, please refer to code defined below.
// ====================================================================

// 3. Close the specified card reader
int DIC_Close(int hic);
// Input:
// hic: Card handle, must be consistent with the hic returned by DIC_Open. 
// Return:
// An error code , for detailed descriptions, please refer to code defined below.

// ====================================================================

// 4. Create virtual IC card file(*virtual card private instruction)
int DIC_Create(char* filename, char* mfname, char* atr, WORD atrlen);
// Input:
// filename:	Path file name to be created 
// mfname:		Main volume name
// atr:			Software developer information 
// atrlen:		Software developer information length

// Return :
// An error code , for detailed descriptions, please refer to code defined below.

// ======================================================================

// 5. Get the dynamic library version
int DIC_GetVersion(char* ver);
// Input:
// ver: returns the version number of the dynamic library, the version content is:
//    [V/R]xx.yy
// For example, R01.10 means it is "Real card dynamic library Version 1.10 "
//              R03.02 indicats "Virtual card dynamic library Version 3.02 "

// Return:
//	An error code , for detailed descriptions, please refer to code defined below.

// ======================================================================

// 6. Complete operations on the specified device 
int DIC_Command(int hic, int cmd, void* cmddata);
// Input
// hic		Device handle 
// cmd		Specific operation, different constants represent different operations, for details, see below.
// cmddata	Input/output data related to the command£¬see comments on below.

// ======================================================================

// 7. Find a dongle that has specific management code 
int DIC_FindByMgrCode(void * pMgrCode);
// Input:
// pMgrCode	a pointer to DICST_ManagerCode structure
// 
// Return:
// The same as DIC_Find

// Command 
#define GET_CARD_INFO				0x02000000
#define GET_HARDWARE_INFO			0x00FF1C00
#define GET_MANAGER_CODE			0x00FF1A00
#define GET_CARD_PRIVILEGE			0x00FF2200
#define GET_REMOTE_INFO				0x00FF1e00
#define SET_REMOTE_INFO				0x00FF1003
#define CHECK_REMOTE_INFO			0x01FF1003
#define GET_CURRENT_DIR				0x00001800
#define SET_CURRENT_DIR				0x0000a400
#define GET_CURRENT_FILE			0x00001801
#define SET_CURRENT_FILE			0x0100a400
#define GET_PARENT_DIR				0x00001802
#define SET_PARENT_DIR				0x0000a402
#define LIST_DIR					0x0000b200
#define	READ_FILE					0x0000b000
#define WRITE_FILE					0x0000d000
#define FORMAT_CARD					0x0000e000
#define CREATE_DIR					0x0000e001
#define CREATE_FILE					0x0100e001
#define REMOVE_DIR					0x0000ee00
#define REMOVE_FILE					0x0000ee01
#define RANDOM						0x00008400
#define RUN							0x03000000
#define CRYPTOTEXT_FILE				0x04000000
#define PLAINTEXT_FILE				0x05000000

#define DESENC						0x00005000
#define DESDEC						0x00005100
#define RSAGENKEY					0x00003300
#define RSAENC						0x00003400
#define RSADEC						0x00003500

#define GETFREESPACE				0X00003000
#define MODULE_GETFILEFLAG			0X00003600
#define MODULE_CRYPTOTEXT_MODFLAG	0x00003700
#define MODULE_PLAINTEXT_MODFLAG	0x00003800
#define MODULE_ENCRYPT_CONTENT		0x00003900

#define SETCOUNTER				0x00005700
#define STEPCOUNTER					0x00005800

#define CHECK_SUPER_PASS			0x00FF1002
#define SET_SUPER_PASS				0x00FFF402
#define GET_UPGRADE_REMOTE_PASS		0x00FF2000

// data related to the command

// data struct for GET_CARD_INFO 
typedef struct
{
	char volume[16];
	char atr[15];
} DICST_CardInfo;

// data struct for GET_HARDWARE_INFO 
typedef struct
{
	DWORD FactoryTime;
	DWORD HardSerial;
	DWORD ShipTime;
	DWORD COSVersion;
} DICST_HardInfo;

// data struct for GET_MANAGER_CODE
typedef struct
{
	WORD Zone;
	WORD Agent;
	WORD User1;
	WORD User2;
} DICST_ManagerCode;

// data for GET_CARD_PRIVILEGE only contain one byte.
// use those bit value to show current privilege status:
#define DICPR_NOPASS		0
#define	DICPR_SUPERPASS		4
#define	DICPR_REMOTEPASS	8

// data struct for GET_REMOTE_INFO, SET_REMOTE_INFO, CHECK_REMOTE_INFO 
typedef struct
{
	DWORD RemoteTag;
	BYTE RemotePass[8];
} DICST_RemoteInfo;

// data struct for GET_UPGRADE_REMOTE_PASS
typedef struct
{
	DWORD RemoteTag;
	DWORD HardSerial;
	BYTE RemotePass[8];
} DICST_Upgrade_RemotePass;

// data struct for GET_CURRENT_DIR, SET_CURRENT_DIR, GET_PARENT_DIR, CREATE_DIR 
typedef struct
{
	WORD		dirid;			// Directory ID
	BYTE		dircla;			// Directory Class
	BYTE		diratrpri;		// Directory attribute & directory security level
	WORD		dirsize;		// Not used
	char		dirname[16];	// Directory name  
} DICST_Dir;

// data struct for GET_CURRENT_FILE, SET_CURRENT_FILE, CREATE_FILE
typedef struct
{
	WORD		fileid;			// File ID
	BYTE		filecla;		// File class
	BYTE		fileatrpri;		// File attribute & file security level
	WORD		filesize;		// File size
	char		filename[17];		// File name
	char        	filepath[128];		// File path
} DICST_File;

// Definitions of diratrpri(directory attribute) and fileatrpri(file attribute)
#define FILEATTR_NORMAL			0x00	// Normal data file
#define FILEATTR_EXEC			0x10	// Executable  
#define FILEATTR_DIR			0x20	// Directory
#define FILEATTR_UPIGNORE		0x40	// If the current security level is already higher than that of the executable -
										// program, the program cannot be executed. 
#define FILEATTR_INTERNAL		0x80	// File used by internal program and can not be operated externally. 
										// If an executable file is marked as internal£¬it has hiding property£¬such files can 
										// be viewed when listing directory only if the super password is verified.
										                                        
// Caution: LIST_DIR Can return both DICST_File or DICST_Dir, determined by FILEATTR_DIR

// Data struct for READ_FILE 
typedef struct
{
	WORD		offset;
	WORD		size;
} DICST_Before_Read_Data;

// Data struct for READ_FILE 
typedef struct
{
	WORD		readedsize;
	char		buffer[1];		// buffer size in readedsize
} DICST_After_Read_Data;

// Data struct for WRITE_FILE
typedef struct
{
	WORD		offset;
	WORD		size;
	char		buffer[1];		// buffer size in size
} DICST_Write_Data;

// Data struct for FORMAT_CARD 
typedef struct
{
	char		volume[16];
	char		atr[15];
	BOOL        isdes;
	BOOL        isfloat;
	BOOL        isExtend;
	BYTE		bModify;
} DICST_System_Info;

// Data struct for RUN_DATA 
typedef struct
{
	WORD RunID;
	WORD ParaSize;
	BYTE Para[1];	// buffer size in ParaSize
} DICST_Before_Run_Data;

// Data struct for RUN_DATA 
typedef struct
{
	WORD ResultSize;
	BYTE Result[1]; // // buffer size in ResultSize
} DICST_After_Run_Data;

// Data struct for SET_SUPER_PASS
typedef struct
{
	BYTE MaxTryTimes;
	BYTE SuperPass[8];
} DICST_SuperPass_Data;

// Data struct for CRYPTOTEXT_FILE 
typedef struct
{
	WORD		fileid;			// File ID
	BYTE		filecla;		// File class
	BYTE		fileatrpri;		// File attribute & file security level
	WORD		filesize;		// File size
	char		filename[16];	// File name
	BYTE		filedata[1];	// File content£¬size in filesize
} DICST_Before_CryptoFile_Data;

// data struct for CRYPTOTEXT_FILE 
typedef struct
{
	WORD ResultSize;
	BYTE Result[1]; // buffer size in ResultSize
} DICST_After_CryptoFile_Data;

// 3 data structs for encryption/decryption algorithms 
typedef struct 
{						//For encrytption	//For decryption
	WORD dataLength;			//Length of data
	WORD KeyFileid;				//File ID of DES Key
	BYTE KeyIndex;				//Key index for this operation£¬
						//Mutiple DES key can be in the same DES key file
						//Different key uses different key index

	char data[1];				//Encrypted data	//Decrypted data
} DICST_Des_Data;

typedef struct 
{						//For encrytption	 	//For decryption
	WORD	KeyFileid;			//File ID of public key file	//File ID of private key file	
	WORD	dataLength;			//Length of data
	char	data[1];			//Encrypted data	//Decrypted data
} DICST_Rsa_Data;

typedef struct 
{
	WORD pubKeyID;				//Public key ID
	WORD pubKeySize;			//key size 512/1024
	WORD pivKeyID;				//Private key ID
} DICST_Rsa_GenKey;


// Returned struct for encryption/decryption algorithms
typedef struct 
{						//For encrytption	 //For decryption
	WORD dataLength;			//Length of data
	char data[1];				//Encrypted data	//Decrypted data
} DICST_After_EncDec_Data;

// Get Module flag
typedef struct 
{
	BYTE NumFileID;		//Number of executable file ID
	WORD fileID[1];		//The executable file ID
}
DICST_Module_GetFLag;

typedef BYTE* FileFlag; // Returned file flag


typedef struct  
{
	WORD fileID;	//File ID
	BYTE flag;	//Flag
}
DICST_PlainText_OneMod;

typedef struct
{
	BYTE dataLen;
	DICST_PlainText_OneMod data[1];
}
DICST_PlainText_ModuleData;

typedef struct 
{
	BYTE numExe;		//number of executable file, max to 3
	BYTE flag;		//the corresponding flags
	WORD exeFileID[3];	//executable file ID
}
DICST_CryptoText_OneMod;

typedef struct  
{
	BYTE nDataLen;
	DICST_CryptoText_OneMod plaintextModuleData[1];
}
DICST_CryptoText_Encrypt;

typedef struct
{
	BYTE dataLen;
	BYTE data[1];
}
DICST_CryptoText_ModuleData;

// ====================================================================

// 7. Get data from return value of DIC_Cmd
int DIC_Get(void* xdata, int p1, int p2, char* buffer);

// Input:
// xdata	Source data buffer
// p1		Mode/shift, if the highest bit is 1£¬it is a mode operation£¬otherwise, it is a user defined shift value
// p2		Return mode / size, the highest 2 bits represent return mode, others indicate the operation size
// buffer	Character type buffer where the data is got

// ====================================================================

// 8. Set data for DIC_Cmd
int DIC_Set(void* xdata, int p1, int p2, int p3, char* buffer);
// Input:
// xdata	Targeted buffer
// p1		Mode/shift, if the highest bit is 1, it is a mode operation£¬otherwise, it is user defined shift value
// p2		Transfer mode/ size, the highest 2 bits specify the transfer mode, others indicate the operation size
// p3		Normally the value that the user needs to store
// buffer	Character type buffer of the source data, it can be NULL

// ====================================================================
// For the p1 parameter of DIC_Get and DIC_Set, some macros are defined to specify the operation objects. 
// The highest bit of the macros is set to 1. The following are the pre-defined macros: 
#define FILL					0x80000000
#define ATR					0x80010000
#define VOLUME					0x80020000
#define FACTORY_TIME				0x80030000
#define HARD_SERIAL				0x80040000
#define SHIP_TIME				0x80050000
#define COS_VERSION				0x80060000
#define ZONE					0x80070000
#define AGENT					0x80080000
#define USER1					0x80090000
#define USER2					0x800A0000
#define DIR_ID					0x800B0000
#define DIR_CLASS				0x800C0000
#define DIR_ATTRIBUTE				0x800D0000
#define DIR_PRIVILEGE				0x800E0000
#define FILE_NO					0x800F0000
#define DIR_NAME				0x80100000
#define FILE_ID					0x80110000
#define FILE_CLASS				0x80120000
#define FILE_ATTRIBUTE				0x80130000
#define FILE_PRIVILEGE				0x80140000
#define FILE_SIZE				0x80150000
#define FILE_NAME				0x80160000
#define WRITE_DATA				0x80170000
#define READ_DATA				0x80180000
#define RUN_DATA				0x80190000
#define RANDOM_SIZE				0x801A0000
#define REMOTE_TAG				0x801B0000
#define REMOTE_PASS				0x801C0000
#define UPGRADE_HARD_SERIAL			0x801D0000
#define UPGRADE_REMOTE_TAG			0x801E0000
#define UPGRADE_REMOTE_PASS			0x801F0000
#define REMOTEPASS_STATUS			0x80200000
#define SUPERPASS_STATUS			0x80210000
#define SUPERPASS_DATA				0x80220000
#define FILE_DATA				0x80230000
#define RELOAD_FLOATLIBS          	  	0x0000e002
#define RELOAD_RSADESLIBS        	   	0x0000e003
#define ISLOADEDFLOATLIBS           		0x0000e004
#define ISLOADEDRSADESLIBS          		0x0000e005 
#define RELOAD_EXTENDFLOATLIBS      		0x0000e006
#define ISLOADEXTENDFLOATLIBS       		0x0000e007

//DICST_Des_Data				//Used by DIC_Set
#define DES_KEYFILEID				0x80240000
#define DES_KEYINDEX				0x80250000
#define DES_DATA				0x80260000

//DICST_After_EncDec_Data			//Used by DIC_Get
#define AFTER_ENCDEC_DATA			0x80270000

//DICST_Rsa_Data				//Used by DIC_Set
#define RSA_KEYFILEID				0x80280000
#define RSA_DATA				0x80290000

//DICST_Rsa_GenKey				//Used by Dic_Set
#define RSA_PUBKEYID				0x802A0000
#define RSA_PUBKEYSIZE				0x802B0000
#define RSA_PIVKEYID				0x802C0000

// ====================================================================
//For the p2 parameter of DIC_Get and DIC_Set, the highest 2 bits indicate data transfer or return mode, 
//the default value of the lower 30-bit is 0, if not, it refers to the operation byte number. 
#define BY_VALUE				0x00000000
#define BY_DOUBLE				0x40000000
#define BY_ARRAY				0x80000000
#define BY_STRING				0xC0000000

// for  DIC_GetData
// BY_VALUE		returned in return value
// BY_DOUBLE	returned in double in string format, such as "328.002"
// BY_ARRAY		returned in pstr 
// BY_STRING	returned in pstr 

// for  DIC_SetData
// BY_VALUE		set data via p3 
// BY_DOUBLE	set double data in string format, such as "328.002"
// BY_ARRAY		set data via pstr 
// BY_STRING	set data via pstr 
// ====================================================================


// Error code ===========================================================

#ifndef SCARD_S_SUCCESS

// Success
#define SCARD_S_SUCCESS 			((DWORD)0x00000000L)

// Internal link check failure
#define SCARD_F_INTERNAL_ERROR			((DWORD)0x80100001L)

// Operation is terminated by user
#define SCARD_E_CANCELLED			((DWORD)0x80100002L)

// Incorrect operation handle
#define SCARD_E_INVALID_HANDLE			((DWORD)0x80100003L)

// Incorrect parameter
#define SCARD_E_INVALID_PARAMETER		((DWORD)0x80100004L)

// Registered startup information lost or invalid 
#define SCARD_E_INVALID_TARGET			((DWORD)0x80100005L)

// Memory not enough for command completion 
#define SCARD_E_NO_MEMORY			((DWORD)0x80100006L)

// Internal overtime
#define SCARD_F_WAITED_TOO_LONG			((DWORD)0x80100007L)

// User specified buffer is too small, not enough for the return data
#define SCARD_E_INSUFFICIENT_BUFFER		((DWORD)0x80100008L)

// Unknown card reader
#define SCARD_E_UNKNOWN_READER			((DWORD)0x80100009L)

// Over user specified time
#define SCARD_E_TIMEOUT				((DWORD)0x8010000AL)

// Card occupied by other connection
#define SCARD_E_SHARING_VIOLATION		((DWORD)0x8010000BL)

// No card in the card reader
#define SCARD_E_NO_SMARTCARD			((DWORD)0x8010000CL)

// Unknown card type
#define SCARD_E_UNKNOWN_CARD			((DWORD)0x8010000DL)

// Card reader can not complete card eject operation
#define SCARD_E_CANT_DISPOSE			((DWORD)0x8010000EL)

// Current card does not support user specified communication protocol
#define SCARD_E_PROTO_MISMATCH			((DWORD)0x8010000FL)

// Card is not ready for receiving commands
#define SCARD_E_NOT_READY			((DWORD)0x80100010L)

// Some variable values are not suitable
#define SCARD_E_INVALID_VALUE			((DWORD)0x80100011L)

// Operation is terminated by system, it may be necessary to log on again or shut down. 
#define SCARD_E_SYSTEM_CANCELLED		((DWORD)0x80100012L)

// Internal communication error
#define SCARD_F_COMM_ERROR			((DWORD)0x80100013L)

// Unknown internal error
#define SCARD_F_UNKNOWN_ERROR			((DWORD)0x80100014L)

// Invalid vendor information
#define SCARD_E_INVALID_ATR			((DWORD)0x80100015L)

// User attempts to terminate some process not available
#define SCARD_E_NOT_TRANSACTED			((DWORD)0x80100016L)

// The specifiied card reader is not ready
#define SCARD_E_READER_UNAVAILABLE		((DWORD)0x80100017L)

// Operation is terminated, service program is allowed to exit
#define SCARD_P_SHUTDOWN			((DWORD)0x80100018L)

// PCI receiving buffer is too small
#define SCARD_E_PCI_TOO_SMALL			((DWORD)0x80100019L)

// Card reader drive does not support the current card reader
#define SCARD_E_READER_UNSUPPORTED		((DWORD)0x8010001AL)

// Card reader driver program can not create an exclusive name, card reader with the same name already exists
#define SCARD_E_DUPLICATE_READER		((DWORD)0x8010001BL)

// Card is not supported by the current card reader
#define SCARD_E_CARD_UNSUPPORTED		((DWORD)0x8010001CL)

// Smart card service is not enabled
#define SCARD_E_NO_SERVICE			((DWORD)0x8010001DL)

// Smart card service is disabled
#define SCARD_E_SERVICE_STOPPED			((DWORD)0x8010001EL)

// Some unexpected smart card error
#define SCARD_E_UNEXPECTED			((DWORD)0x8010001FL)

// Smart card software developer information not accessible
#define SCARD_E_ICC_INSTALLATION		((DWORD)0x80100020L)

// Smart card vendor information not accessible
#define SCARD_E_ICC_CREATEORDER			((DWORD)0x80100021L)

// Current smart card does not support the function user demanded
#define SCARD_E_UNSUPPORTED_FEATURE		((DWORD)0x80100022L)

// The specified directory does not exist
#define SCARD_E_DIR_NOT_FOUND			((DWORD)0x80100023L)

// The specified file does not exist
#define SCARD_E_FILE_NOT_FOUND			((DWORD)0x80100024L)

// The specified directory is no longer a valid directory
#define SCARD_E_NO_DIR				((DWORD)0x80100025L)

// The specified file is no longer a valid file, no file selected
#define SCARD_E_NO_FILE				((DWORD)0x80100026L)

// The file denies any access
#define SCARD_E_NO_ACCESS			((DWORD)0x80100027L)

// Card is full, no more data write
#define SCARD_E_WRITE_TOO_MANY			((DWORD)0x80100028L)

// File pointer error
#define SCARD_E_BAD_SEEK			((DWORD)0x80100029L)

// PIN code error
#define SCARD_E_INVALID_CHV			((DWORD)0x8010002AL)

// An unrecognized error code is returned from smart card service
#define SCARD_E_UNKNOWN_RES_MNG			((DWORD)0x8010002BL)

// The certificate requested does not exist
#define SCARD_E_NO_SUCH_CERTIFICATE		((DWORD)0x8010002CL)

// The certificate requested is not available
#define SCARD_E_CERTIFICATE_UNAVAILABLE		((DWORD)0x8010002DL)

// No card reader found
#define SCARD_E_NO_READERS_AVAILABLE		((DWORD)0x8010002EL)

// Data loss during smart card communication, please try again
#define SCARD_E_COMM_DATA_LOST			((DWORD)0x8010002FL)

// The key file requested does not exist
#define SCARD_E_NO_KEY_CONTAINER		((DWORD)0x80100030L)

// Card reader can not communicate with the card due to software developer information conflict
#define SCARD_W_UNSUPPORTED_CARD		((DWORD)0x80100065L)

// Card does not respond to reset
#define SCARD_W_UNRESPONSIVE_CARD		((DWORD)0x80100066L)

// Card no power
#define SCARD_W_UNPOWERED_CARD			((DWORD)0x80100067L)

// Card is reset, shared information is invalid
#define SCARD_W_RESET_CARD			((DWORD)0x80100068L)

// Card is removed
#define SCARD_W_REMOVED_CARD			((DWORD)0x80100069L)

// Access is denied due to security rules
#define SCARD_W_SECURITY_VIOLATION		((DWORD)0x8010006AL)

// PIN code is not verified, access is denied
#define SCARD_W_WRONG_CHV			((DWORD)0x8010006BL)

// Max. PIN code retry number is reached, access is denied
#define SCARD_W_CHV_BLOCKED			((DWORD)0x8010006CL)

// The last smart card file is reached, no more file available
#define SCARD_W_EOF				((DWORD)0x8010006DL)

// Operation is terminated by user
#define SCARD_W_CANCELLED_BY_USER		((DWORD)0x8010006EL)

// Smart card PIN is not set
#define SCARD_W_CARD_NOT_AUTHENTICATED		((DWORD)0x8010006FL)

#endif // SCARD_S_SUCCESS


// Extended error code  =========================================================

// File already exists
#define SCARD_E_FILE_EXISTS			((DWORD)0xA0100001L)

// Card-resident memory operation error
#define SCARD_E_EPROM_ERROR			((DWORD)0xA0100002L)

// User specified CLA is invalid
#define SCARD_E_INVALID_CLA			((DWORD)0xA0100003L)

// User specified INS is invalid
#define SCARD_E_INVALID_INS			((DWORD)0xA0100004L)

// Virtual machine address out ranged/abnormal 
#define SCARD_E_VM_ADDRESS_ERROR		((DWORD)0xA0100005L)

// 0 division error
#define SCARD_E_ZERO_DIVIDE			((DWORD)0xA0100006L)

// Card is not inserted to the proper place
#define SCARD_E_WRONG_POSITION			((DWORD)0xA0100007L)

// Card is in an unknown status
#define SCARD_E_UNKNOWN_STATE			((DWORD)0xA0100008L)

// Card is not opened
#define SCARD_E_CARD_NOT_OPENED			((DWORD)0xA0100009L)

// Unknown command
#define SCARD_E_UNKNOWN_COMMAND			((DWORD)0xA010000AL)

// Super password reset number is 0
#define SCARD_E_ZERO_TRYTIME			((DWORD)0xA010000BL)

// Too many devices are opened
#define SCARD_E_TOO_MANY_DEVICE			((DWORD)0xA010000CL)

// Invalid instruction.
#define	SCARD_E_INVALID_INSTRUCTION		((DWORD)0xA010000DL)

// card still have more data to return 
#define SCARD_W_RESPONSE			((DWORD)0xA01000FFL)

// returned data length
#define RETURN_LENGTH				((DWORD)0xA0100014)

// Error codes exclusive for virtual devices=================================================

// Virtual card file creation failure
#define SCARD_E_FILE_CREATE_FAILED		((DWORD)0xA0101001L)

// Virtual card file open failure
#define SCARD_E_FILE_OPEN_FAILED		((DWORD)0xA0101002L)

#define SCARD_E_FLOAT_NOT_FOUND         	((DWORD)0x0000e00aL)

//RSA_DES library not found
#define SCARD_E_RSADES_NOT_FOUND        	((DWORD)0x0000e00bL)

#define SCARD_E_LIBS_ERROR              	((DWORD)0x0000e00cL)

#define SCARD_E_FLOAT_NOT_FOUND         	((DWORD)0x0000e00aL)

//RSA_DES library not found
#define SCARD_E_RSADES_NOT_FOUND        	((DWORD)0x0000e00bL)

#define SCARD_E_LIBS_ERROR              	((DWORD)0x0000e00cL)

#define SCARD_E_EXTENDFLOAT_NOT_FOUND   	((DWORD)0x0000e00dL)

//Extended error codes for ROCKEY

//Cannot find ROCKEY
#define ROCKEY_K_NOT_FOUND               	((DWORD)0xF0100001L)

//Search ROCKEY failed	
#define ROCKEY_K_QUERY_FAILED            	((DWORD)0xF0100002L)

//Write ROCKEY failed
#define ROCKEY_K_WRITE_FAILED            	((DWORD)0xF0100003L)

//Read ROCKEY failed
#define ROCKEY_K_READ_FAILED            	 ((DWORD)0xF0100004L)

/*******Rockey6Smart V2.0 new Function*******/

int DIC_GetCardInfo(int hic, char strVolume[], char strManufactureInfo[]);
int  DIC_GetHardwareInfo(int hic, 
							   DWORD *pdwProducedTime, 
							   DWORD *pdwSerial, 
							   DWORD *pdwSalesTime, 
							   DWORD *pdwCOSVersion);
int  DIC_GetManagerCode(int hic, 
							  WORD *pwZoneCode, 
							  WORD *pwAgentCode, 
							  WORD *pwUser1Code, 
							  WORD *pwUser2Code);
int  DIC_GetPasswordVerifyState(int hic, int *piState);
int  DIC_GetRemoteInfo(int hic, DWORD *pdwFlag, BYTE abyPassword[]);
int  DIC_SetRemoteInfo(int hic, DWORD dwFlag, const BYTE abyPassword[]);
int  DIC_CheckRemoteInfo(int hic, DWORD dwFlag, const BYTE byPassword[]);
int  DIC_GetCurrentDir(int hic, 
							 WORD *pwDirID, 
							 BYTE *pbyDirType, 
							 BYTE *pbyDirAttribute, 
							/* BYTE *pbyDirSecurity, */
							 char strDirName[]);
int  DIC_SetCurrentDir(int hic, 
							 WORD wDirID,
							 char strDirName[]);
int  DIC_GetCurrentFile(int hic, 
							  WORD *pwFileID, 
							  BYTE *pbyFileType, 
							  WORD *pwFileSize, 
							  BYTE *pbyFileAttribute, 
							  BYTE *pbyFileSecurity, 
							  char strFileName[]);
int  DIC_SetCurrentFile(int hic, 
							  WORD wFileID,
							  char strFileName[]);
int  DIC_GetParentDir(int hic, 
							WORD *pwDirID, 
							BYTE *pbyDirType, 
							BYTE *pbyDirAttribute, 
						/*	BYTE *pbyDirSecurity,*/ 
							char strDirName[]);
int  DIC_SetParentDir(int hic);
int  DIC_FindFirstFileOrDir(			int hic, 
								  int *piPosition, 
								  WORD *pwID, 
								  BYTE *pbyType, 
								  BYTE *pbyAttribute, 
								  BYTE *pbySecurity, 
								  char strName[]);
int  DIC_FindNextFileOrDir(			int hic, 
								 int *piPosition, 
								 WORD *pwID, 
								 BYTE *pbyType, 
								 BYTE *pbyAttribute, 
								 BYTE *pbySecurity, 
								 char strName[]);
int  DIC_ReadFile(int hic, WORD wOffset, WORD *pwRead, char *pBuf);
int  DIC_WriteFile(int hic, WORD wOffset, WORD wWrite, const char *pBuf);
int  DIC_FormatCard(int hic, char strVolume[], char strManufactureInfo[]);
int  DIC_CreateDir(int hic, 
						 WORD wDirID, 
						 BYTE byDirType, 
						 BYTE byDirAttribute, 
						/* BYTE byDirSecurity, */
						 const char strDirName[]);
int  DIC_CreateFile(int hic, 
						  WORD wFileID, 
						  BYTE byFileType, 
						  BYTE byFileAttribute, 
						  WORD wFileSize, 
						  BYTE byFileSecurity, 
						  const char strFileName[]);
int  DIC_DeleteDir(int hic);
int  DIC_DeleteFile(int hic);
int  DIC_Random(int hic, BYTE byRandomLength, BYTE abyRandom[]);
int  DIC_Run(int hic, WORD wRunFileNameID, WORD wParamSize, const char *cpcParam, WORD *pwRetDataLength, char *pcRetData);
int  DIC_EncryptFileUseTea(int hic, 
								 WORD wFileID, 
								 BYTE byFileType, 
								 BYTE byFileAttribute, 
								 WORD *pwFileSize, 
								 BYTE byFileSecurity, 
								 const char strFileName[], 
								 const char *cpcFileData,
								 char *pcEncData);
int  DIC_TeaEncryptedFile2PlainFile(int hic, char *cmddata);
int  DIC_CheckSuperPassword(int hic, BYTE abySuperPW[]);
int  DIC_SetSuperPassword(int hic, BYTE byTryTimesCeiling, BYTE abySuperPW[]);
int  DIC_GetRemoteUpgradePassword(int hic, DWORD dwRemoteFlag, DWORD dwHardSerial, BYTE abyRemotePW[]);
int  DIC_GenerateRsaKey(int hic, WORD wPublicKeyFileID, WORD wPublicKeySize, WORD wPrivateKeyFileID);
int  DIC_GenerateRsaPublicKeyFile(int hic, WORD wPublicKeyFileID, WORD wPublicKeySize/*512/1024/2048*/, const BYTE *cpbyPublicKeyData);
int  DIC_GenerateRsaPrivateKeyFile(int hic, WORD wPrivateKeyFileID, WORD wPrivateKeySize/*512/1024/2048*/, const BYTE *cpbyPrivateKeyDataD, const BYTE *cpbyPrivateKeyDataN);
int  DIC_RsaEncrypt(int hic, WORD wPublicKeyFileID, WORD wDataLength, const char *cpcDataSrc, char *pcDataEnc, BOOL bIsDataSrcFromOpenssl);
int  DIC_RsaDecrypt(int hic, WORD wPrivateKeyFileID, WORD wDataLength, const char *cpcDataSrc, char *pcDataDec, BOOL bIsDataSrcFromOpenssl);
int  DIC_DesEncrypt(int hic, WORD wKeyFileID, BYTE byKeyIndex, WORD wDataLength, const char *cpcDataSrc, char *pcDataEnc);
int  DIC_DesDecrypt(int hic, WORD wKeyFileID, BYTE byKeyIndex, WORD wDataLength, const char *cpcDataSrc, char *pcDataDec);
int  DIC_GenerateDesKeyFile(int hic, WORD wKeyFileID, WORD wKeyDataLength, char *pcKeyData);
int  DIC_GetFreeSpace(int hic, DWORD *pdwFreeSpace);
int  DIC_SetCounter(int hic, DWORD dwInit);
int  DIC_DecreaseCounter(int hic);

#ifdef __cplusplus
}
#endif

#endif //__DIC32_H__
