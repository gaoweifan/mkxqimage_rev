/* This file was generated by the Hex-Rays decompiler.
   Copyright (c) 2007-2020 Hex-Rays <info@hex-rays.com>

   Detected compiler: GNU C++
*/
//#include "defs.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/reboot.h>
#include <sys/ioctl.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/pem.h>

typedef struct __attribute__((aligned(4))) 
{
  uint32_t magic;
  uint32_t rsaLen;
  uint32_t crc32sum;
  uint16_t romType;
  int16_t model;
  uint32_t fileOffs[8];
} MiRomHdr;

typedef struct
{
  uint16_t magic;
  uint16_t rsvd0;
  uint32_t flashaddr;
  uint32_t filesize;
  uint16_t mtdid;
  uint16_t unk5;
  char name[32];
} MiRomFile;



//-------------------------------------------------------------------------
// Function declarations
void error_exit();
size_t util_get_file_len(FILE *a1);
int util_check_model_idx(int16_t model_idx);
int main(int argc, const char **argv, const char **envp);
int for_each_file(FILE *f, int (*callback)(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata), void *userData);
int load_image(FILE *f, int disableVerify);
int command_verify_image(const char *filename);
int extract_normal_rom_callback(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata);
int command_extract_file(const char *filename, const char *sshFile, int disableVerify, const char *fname, int outputToFile);
int flash_mtd_callback(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata);
int command_mtd_flash(const char *filename, int needVerify);
int dump_file_desc(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata);
int command_list_files(const char *image, const char *a2);
int get_version_callback(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata);
int command_get_image_version(const char *a1);
int command_get_ssh_key();
unsigned int crc32_update(uint8_t *buf, int64_t size, unsigned int state);
uint32_t calc_crc32(FILE *f, int start, unsigned int size);
int64_t init_pkey(EVP_MD_CTX *ctx, RSA **pem, EVP_PKEY **ppkey, int isSsh, char isSHA256); // idb
int j_verify_rsa(FILE *f, unsigned int start, unsigned int size, const unsigned char *sigbuf, unsigned int siglen, unsigned int isSsh, char isSHA256);
int verify_rsa(FILE *f, int start, unsigned int size, const unsigned char *sigbuf, unsigned int siglen, unsigned int isSsh, char isSHA256);
int64_t write_buf(int fd, void *buf, unsigned int size);
int open_and_erase_mtd(unsigned int mtdId);
int64_t calcMd5(unsigned char *md5Output, const char *data);
int64_t transpostGuid(uint8_t *a1, char *outbuf);
int64_t get_ssh_key();
int extract_ssh_rom_callback(MiRomHdr *hdr, int64_t fileId, MiRomFile *fildhdr, FILE *f, void *SN_str);

//-------------------------------------------------------------------------
// Data declarations

uint32_t crc32_table[256] =
{
 0, 0x77073096, 0xEE0E612C, 0x990951BA,
 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
 0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
 0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
 0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
 0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
 0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
 0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};


char *model_nr[30] =
{
  "NULL",
  "NULL",
  "NULL",
  "R1CM",
  "R2D",
  "R1CL",
  "R2CM",
  "R3",
  "R3D",
  "R3L",
  "R3P",
  "P01",
  "R3A",
  "R3G",
  "R4",
  "R4C",
  "D01",
  "R4A",
  "R4CM",
  "R4AC",
  "R3GV2",
  "R2600",
  "R2100",
  "RM2100",
  "R3600",
  "R1350",
  "R2200",
  "R2350",
  "IR1200G",
  "R1800"
}; // weak
int max_model_nr = 30; // weak

//----- (0000000000401730) ----------------------------------------------------
void error_exit()
{
  fwrite("Ouch!\n", 1u, 6u, stderr);
  exit(1);
}

//----- (00000000004017A4) ----------------------------------------------------
size_t util_get_file_len(FILE *a1)
{
  size_t v3; // [xsp+28h] [xbp+28h]
  int v4; // [xsp+2Ch] [xbp+2Ch]

  v4 = ftell(a1);
  fseek(a1, 0LL, 2);
  v3 = ftell(a1);
  fseek(a1, v4, 0);
  return v3;
}

//----- (00000000004017F8) ----------------------------------------------------
int util_check_model_idx(int16_t model_idx)
{
#ifdef ONDEVICE
  int result; // w0
  char v3[8]; // [xsp+20h] [xbp+20h] BYREF
  FILE *stream; // [xsp+28h] [xbp+28h]

  memset(v3, 0, 8);
  stream = popen("cat /proc/xiaoqiang/model", "r");
  if ( stream )
  {
    if ( fgets(v3, 8, stream) )
    {
      pclose(stream);
      if ( model_idx < max_model_nr )
      {
        result = strcmp(v3, model_nr[model_idx]);
      }
      else
      {
        fprintf(
          stderr,
          "Illegal model: model_idx=%d,max_model_nr=%d \n",
          (unsigned int)model_idx,
          (unsigned int)max_model_nr);
        result = -1;
      }
    }
    else
    {
      fwrite("Failed to get model \n", 1u, 0x15u, stderr);
      result = -1;
    }
  }
  else
  {
    fwrite("Failed to run command\n", 1u, 0x16u, stderr);
    result = -1;
  }
  return result;
#else
	if ( model_idx < max_model_nr )
	{
	printf("%s\n", model_nr[model_idx]);
	}
	else if(model_idx==61){
	  printf("RB06\n");
	}
	else if(model_idx==37){
	  printf("RA70\n");
	}
  else if(model_idx==32){
	  printf("RA69\n");
	}
	else
	{
	printf(
	  "Illegal model: model_idx=%d,max_model_nr=%d \n",
	  (unsigned int)model_idx,
	  (unsigned int)max_model_nr);
	}
  
  return 0;
#endif
}

//----- (0000000000401918) ----------------------------------------------------
int main(int argc, const char **argv, const char **envp)
{
  int v6; // [xsp+20h] [xbp+20h]
  int ret; // [xsp+24h] [xbp+24h]
  const char *filename; // [xsp+28h] [xbp+28h]
  const char *a2; // [xsp+30h] [xbp+30h]
  const char *file; // [xsp+38h] [xbp+38h]
  int toStdout; // [xsp+44h] [xbp+44h]
  int disableVerify; // [xsp+48h] [xbp+48h]
  int mode; // [xsp+4Ch] [xbp+4Ch]

  mode = -1;
  disableVerify = 0;
  toStdout = 0;
  file = 0LL;
  a2 = 0LL;
  filename = 0LL;
  OPENSSL_add_all_algorithms_noconf();
  while ( 1 )
  {
    v6 = getopt(argc, (char *const *)argv, "-v:x:w:c:V:Io:s:m:t:p:f:S:rn");
    if ( v6 == -1 )
      break;
    switch ( v6 )
    {
      case 'I':
        mode = 6;
        continue;
      case 'S':
        a2 = optarg;
        continue;
      case 'V':
        mode = 5;
        filename = optarg;
        continue;
      case 'c':
        mode = 4;
        filename = optarg;
        continue;
      case 'f':
        if ( mode != 2 && mode != 4 )
          continue;
        if ( file )
        {
          fwrite("Multiple files specified\n", 1u, 0x19u, stderr);
          return -22;
        }
        file = optarg;
        break;
      case 'n':
        toStdout = 1;
        continue;
      case 'r':
        disableVerify = 1;
        continue;
      case 'v':
        mode = 1;
        filename = optarg;
        continue;
      case 'w':
        mode = 3;
        filename = optarg;
        continue;
      case 'x':
        mode = 2;
        filename = optarg;
        continue;
      default:
        error_exit();
    }
  }
  switch ( mode )
  {
    case 1:
      ret = command_verify_image(filename);
      break;
    case 2:
      ret = command_extract_file(filename, a2, disableVerify, file, toStdout);
      break;
    case 3:
      ret = command_mtd_flash(filename, disableVerify);
      break;
    case 4:
      ret = command_list_files(filename, file);
      break;
    case 5:
      ret = command_get_image_version(filename);
      break;
    case 6:
      ret = command_get_ssh_key();
      break;
    default:
      error_exit();
  }
  return ret;
}

//----- (0000000000401BAC) ----------------------------------------------------
int for_each_file(FILE *f, int (*callback)(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata), void *userData)
{
  MiRomFile filehdr; // [xsp+30h] [xbp+30h] BYREF
  MiRomHdr buf; // [xsp+60h] [xbp+60h] BYREF
  size_t readret; // [xsp+90h] [xbp+90h]
  unsigned int off; // [xsp+94h] [xbp+94h]
  int ret; // [xsp+98h] [xbp+98h]
  int fileid; // [xsp+9Ch] [xbp+9Ch]

  ret = 0;
  off = 0;
  if ( !callback )
    return -1;
  rewind(f);
  readret = fread(&buf, 1u, 0x30u, f);
  if ( ferror(f) )
    return 0xFFFFFFFB;
  for ( fileid = 0; fileid <= 7; ++fileid )
  {
    off = buf.fileOffs[fileid];
    if ( !off )
      break;
    ret = fseek(f, off, 0);
    if ( ret )
      break;
    readret = fread(&filehdr, 1u, 0x30u, f);
    if ( readret != 48 )
      return -5;
    ret = callback(&buf, (unsigned char)fileid, &filehdr, f, userData);
    if ( ret )
      return ret;
  }
  return ret;
}

//----- (0000000000401D08) ----------------------------------------------------
int load_image(FILE *fptr, int disableVerify)
{
  unsigned char sigbuf[4096]; // [xsp+20h] [xbp+20h] BYREF
  size_t sigsize[4]; // [xsp+1020h] [xbp+1020h] BYREF
  MiRomHdr hdr; // [xsp+1030h] [xbp+1030h] BYREF
  uint32_t crcret; // [xsp+1064h] [xbp+1064h]
  short model_idx; // [xsp+106Ah] [xbp+106Ah]
  size_t ret; // [xsp+106Ch] [xbp+106Ch]
  size_t len; // [xsp+1070h] [xbp+1070h]
  uint16_t v12; // [xsp+1076h] [xbp+1076h]
  unsigned int isSsh; // [xsp+1078h] [xbp+1078h]
  int verify_ret; // [xsp+107Ch] [xbp+107Ch]
  char isSHA256;
  FILE *f = fptr;

  memset(sigbuf, 0, sizeof(sigbuf));
  v12 = -1;
  verify_ret = 0;
  len = 0;
  isSsh = 0;
  rewind(f);
  //ret = fread(&hdr, 1u, 0x30u, f);
  //if ( ret != 0x30 )
  if ( fread(&hdr, 1u, 0x38u, f) != 56 )//new
    return 0xFFFFFFFB;
  if ( hdr.magic != '1RDH' )
  {
    fwrite("Invalid image block\n", 1u, 0x14u, stderr);
    return -22;
  }
  model_idx = hdr.model;
  if ( util_check_model_idx(hdr.model) )
  {
    fwrite("Model mismatch\n", 1u, 0xFu, stderr);
    return -22;
  }
  if(model_idx==61)
    isSHA256=1;
  else
    isSHA256=0;
  len = util_get_file_len(f);
  rewind(f);
  crcret = calc_crc32(f, 12, len - 12);
  if ( hdr.crc32sum != crcret )
  {
    fwrite("Image checksum is invalid\n", 1u, 0x1Au, stderr);
    return -22;
  }
  v12 = hdr.romType;
  if ( hdr.romType == 12 )
    isSsh = 1;
  if ( disableVerify )
  {
    verify_ret = 0;
  }
  else
  {
    fseek(f, hdr.rsaLen, SEEK_SET);
    ret = fread(sigsize, 1u, 0x10u, f);
    if ( ret != 16 )
      return -5;
    ret = fread(sigbuf, 1u, sigsize[0], f);
    if ( sigsize[0] != ret )
      return -5;
    rewind(f);
    verify_ret = j_verify_rsa(f, 12u, hdr.rsaLen - 12, sigbuf, sigsize[0], isSsh, isSHA256);
  }
  if ( !verify_ret )
    return 0;
  fwrite("Image verify failed, not formal image\n", 1u, 0x26u, stderr);
  return -22;
}

//----- (0000000000401FBC) ----------------------------------------------------
int command_verify_image(const char *filename)
{
  FILE *stream; // [xsp+20h] [xbp+20h]
  int v4; // [xsp+2Ch] [xbp+2Ch]

  stream = fopen(filename, "r");
  if ( stream )
  {
    v4 = load_image(stream, 0);
    fclose(stream);
  }
  else
  {
    fprintf(stderr, "can not open \"%s\" for reading\n", filename);
    v4 = -2;
  }
  return v4;
}

//----- (0000000000402048) ----------------------------------------------------
int extract_normal_rom_callback(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata)
{
  int64_t v5; // x0
  size_t v9; // [xsp+4Ch] [xbp+4Ch]
  void *ptr; // [xsp+50h] [xbp+50h]
  char *fname; // [xsp+58h] [xbp+58h]
  int v13; // [xsp+6Ch] [xbp+6Ch]
  size_t chunksize; // [xsp+70h] [xbp+70h]
  uint32_t filesize; // [xsp+74h] [xbp+74h]
  FILE *s; // [xsp+78h] [xbp+78h]

  v13 = 0;
  fname = *(char **)userdata;
  if ( fildhdr->filesize <= 7 || fildhdr->magic != 0xBABE )
    return 0;
  if ( fname && strcmp(fildhdr->name, fname) )
    return 0;
  if ( *((uint32_t *)userdata + 2) )
  {
    s = (FILE *)stdout;
  }
  else
  {
    s = fopen(fildhdr->name, "w");
    if ( !s )
    {
      fprintf(stderr, "can not open \"%s\" for writing\n", fildhdr->name);
      return -5;
    }
  }
  ptr = malloc(0x10000u);
  if ( ptr )
  {
    filesize = fildhdr->filesize;
    while ( filesize )
    {
      v9 = fread(ptr, 1u, 0x10000u, f);
      if ( filesize < v9 )
      {
        chunksize = filesize;
        v5 = fwrite(ptr, filesize, 1u, s);
      }
      else
      {
        chunksize = v9;
        v5 = fwrite(ptr, v9, 1u, s);
      }
      if ( !v5 || fflush(s) )
      {
        fwrite("fwrite failed\n", 1u, 0xEu, stderr);
        v13 = -5;
        goto LABEL_26;
      }
      filesize -= chunksize;
      if ( feof(f) && filesize )
      {
        fwrite("File too short while required write length too long\n", 1u, 0x34u, stderr);
        v13 = -5;
        goto LABEL_26;
      }
    }
    if ( fname )
      v13 = 1;
LABEL_26:
    free(ptr);
  }
  else
  {
    fwrite("malloc failed\n", 1u, 0xEu, stderr);
    v13 = -12;
  }
  if ( !*((uint32_t *)userdata + 2) )
    fclose(s);
  return v13;
}
// 414DC8: using guessed type int64_t stdout;

//----- (00000000004022F8) ----------------------------------------------------
int command_extract_file(const char *filename, const char *sshFile, int disableVerify, const char *fname, int outputToFile)
{
  void *v10[2]; // [xsp+38h] [xbp+38h] BYREF
  MiRomHdr hdr; // [xsp+48h] [xbp+48h] BYREF
  size_t v12; // [xsp+78h] [xbp+78h]
  uint16_t v13; // [xsp+7Eh] [xbp+7Eh]
  FILE *stream; // [xsp+80h] [xbp+80h]
  int ret; // [xsp+8Ch] [xbp+8Ch]

  ret = 0;
  v13 = 0;
  v10[1] = (void *)outputToFile;
  v10[0] = fname;
  stream = fopen(filename, "r");
  if ( stream )
  {
    ret = load_image(stream, disableVerify);
    if ( !ret )
    {
      rewind(stream);
      //v12 = fread(&hdr, 1u, 0x30u, stream);
      //if ( v12 == 48 )
	  if ( fread(&hdr, 1u, 0x38u, stream) == 56 )//new
      {
        v13 = hdr.romType;
        if ( hdr.romType == 12 )
          ret = for_each_file(stream, extract_ssh_rom_callback, (void *)sshFile);
        else
          ret = for_each_file(stream, extract_normal_rom_callback, v10);
        if ( fname )
        {
          if ( ret <= 0 )
          {
            if ( !ret )
              ret = -2;
          }
          else
          {
            ret = 0;
          }
        }
      }
      else
      {
        ret = -5;
      }
    }
    fclose(stream);
  }
  else
  {
    fprintf(stderr, "can not open \"%s\" for reading\n", filename);
    ret = -2;
  }
  return ret;
}

//----- (0000000000402484) ----------------------------------------------------
int flash_mtd_callback(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata)
{
  unsigned int v8; // [xsp+44h] [xbp+44h]
  void *buf; // [xsp+48h] [xbp+48h]
  int fd; // [xsp+50h] [xbp+50h]
  int v11; // [xsp+54h] [xbp+54h]
  unsigned int size; // [xsp+58h] [xbp+58h]
  unsigned int filesize; // [xsp+5Ch] [xbp+5Ch]

  v11 = 0;
  if ( fildhdr->filesize <= 7 || fildhdr->magic != 0xBABE )
    return v11;
  if ( fildhdr->mtdid == 0xFFFF )
  {
    printf("Skip offset[%hhu]\n", (unsigned char)fileid);
  }
  else
  {
    fd = open_and_erase_mtd(fildhdr->mtdid);
    buf = malloc(0x10000u);
    if ( buf )
    {
      filesize = fildhdr->filesize;
      while ( filesize )
      {
        v8 = fread(buf, 1u, 0x10000u, f);
        if ( filesize < v8 )
        {
          size = filesize;
          write_buf(fd, buf, filesize);
        }
        else
        {
          size = v8;
          write_buf(fd, buf, v8);
        }
        filesize -= size;
        if ( feof(f) && filesize )
        {
          fwrite("File too short while required write length too long\n", 1u, 0x34u, stderr);
          free(buf);
          v11 = -5;
          break;
        }
      }
    }
    else
    {
      fwrite("malloc failed\n", 1u, 0xEu, stderr);
      v11 = -12;
    }
    close(fd);
  }
  return v11;
}

//----- (0000000000402628) ----------------------------------------------------
int command_mtd_flash(const char *filename, int needVerify)
{
  FILE *stream; // [xsp+20h] [xbp+20h]
  int v6; // [xsp+2Ch] [xbp+2Ch]

  stream = fopen(filename, "r");
  if ( stream )
  {
    v6 = load_image(stream, needVerify);
    if ( !v6 )
    {
      system("nvram set flashing=1");
      system("nvram commit");
      v6 = for_each_file(stream, flash_mtd_callback, 0LL);
      if ( !v6 )
      {
        system("nvram unset flashing");
        system("nvram commit");
        fwrite("system will reboot\n", 1u, 0x13u, stderr);
        reboot(0x1234567);
      }
    }
    fclose(stream);
  }
  else
  {
    fprintf(stderr, "can not open \"%s\" for reading\n", filename);
    v6 = -2;
  }
  return v6;
}

//----- (0000000000402754) ----------------------------------------------------
int dump_file_desc(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata)
{
  if ( !(uint8_t)fileid )
  {
    if ( hdr->magic != '1RDH' )
    {
      printf("Invalid MAGIC 0x%x, 0x%x\n", hdr->magic, 827475016LL);
      return 0xFFFFFFEA;
    }
    printf("version %d, file type: %d\n", hdr->model, hdr->romType);
  }
  if ( userdata && strcmp(fildhdr->name, (const char *)userdata) )
    return 0;
  printf("Segment %d: Offset 0x%x\n", (unsigned char)fileid, hdr->fileOffs[(unsigned char)fileid]);
  if ( fildhdr->magic == 0xBABE )
  {
    printf("Filename %s\n", fildhdr->name);
    printf("Flash addr = 0x%x, length = 0x%x, partition = %d\n", fildhdr->flashaddr, fildhdr->filesize, fildhdr->mtdid);
    if ( userdata )
      return 1;
  }
  else
  {
    printf("Invalid sub MAGIC,magic:0x%x le_mag:0x%x rsvd0:0x%x\n", fildhdr->magic, fildhdr->magic, fildhdr->rsvd0);
  }
  return 0;
}

//----- (00000000004028E0) ----------------------------------------------------
int command_list_files(const char *image, const char *a2)
{
  FILE *stream; // [xsp+20h] [xbp+20h]
  int v6; // [xsp+2Ch] [xbp+2Ch]

  stream = fopen(image, "r");
  if ( stream )
  {
    v6 = for_each_file(stream, dump_file_desc, (void *)a2);
    if ( a2 )
    {
      if ( v6 <= 0 )
      {
        if ( !v6 )
          v6 = -2;
      }
      else
      {
        v6 = 0;
      }
    }
    fclose(stream);
  }
  else
  {
    fprintf(stderr, "can not open \"%s\" for reading\n", image);
    v6 = -2;
  }
  return v6;
}

//----- (00000000004029B0) ----------------------------------------------------
int get_version_callback(MiRomHdr *hdr, int64_t fileid, MiRomFile *fildhdr, FILE *f, void *userdata)
{
  char *ptr; // [xsp+50h] [xbp+50h]
  uint32_t size; // [xsp+58h] [xbp+58h]
  int size_4; // [xsp+5Ch] [xbp+5Ch]

  size_4 = 0;
  if ( fildhdr->filesize <= 7 || fildhdr->magic != 47806 )
    return 0;
  size = fildhdr->filesize;
  if ( strcmp(fildhdr->name, "xiaoqiang_version") )
    return size_4;
  if ( malloc(0x10000u) && (ptr = (char *)malloc(size)) != 0LL )
  {
    if ( fread(ptr, 1u, size, f) == size )
    {
      printf("%s", ptr);
      size_4 = 1;
    }
    else
    {
      size_4 = -5;
    }
    free(ptr);
  }
  else
  {
    fwrite("malloc failed\n", 1u, 0xEu, stderr);
    size_4 = -12;
  }
  return size_4;
}

//----- (0000000000402B20) ----------------------------------------------------
int command_get_image_version(const char *a1)
{
  FILE *stream; // [xsp+20h] [xbp+20h]
  int v4; // [xsp+2Ch] [xbp+2Ch]

  stream = fopen(a1, "r");
  if ( stream )
  {
    v4 = for_each_file(stream, get_version_callback, 0LL);
    if ( v4 <= 0 )
    {
      if ( !v4 )
        v4 = -2;
    }
    else
    {
      v4 = 0;
    }
    fclose(stream);
  }
  else
  {
    fprintf(stderr, "can not open \"%s\" for reading\n", a1);
    v4 = -2;
  }
  return v4;
}

//----- (0000000000402BE0) ----------------------------------------------------
int command_get_ssh_key()
{
  get_ssh_key();
  return 0;
}

//----- (0000000000402C78) ----------------------------------------------------
unsigned int crc32_update(uint8_t *buf, int64_t size, unsigned int state)
{
  while ( size )
  {
    state = crc32_table[(unsigned char)(*buf ^ state)] ^ (state >> 8);
    --size;
    ++buf;
  }
  return state;
}

//----- (0000000000402CF4) ----------------------------------------------------
uint32_t calc_crc32(FILE *f, int start, unsigned int size)
{
  uint32_t result; // w0
  unsigned int v6; // [xsp+24h] [xbp+24h]
  uint8_t *ptr; // [xsp+28h] [xbp+28h]
  unsigned int chunk_size; // [xsp+34h] [xbp+34h]
  unsigned int crc; // [xsp+3Ch] [xbp+3Ch]

  crc = -1;
  ptr = malloc(0x10000u);
  if ( ptr )
  {
    fseek(f, start, 0);
    while ( size )
    {
      v6 = fread(ptr, 1u, 0x10000u, f);
      if ( size < v6 )
        chunk_size = size;
      else
        chunk_size = v6;
      crc = crc32_update(ptr, chunk_size, crc);
      size -= chunk_size;
      if ( feof(f) && size )
      {
        fwrite("File too short while required CRC length too long\n", 1u, 0x32u, stderr);
        return 1;
      }
    }
    free(ptr);
    rewind(f);
    result = crc;
  }
  else
  {
    fwrite("malloc failed\n", 1u, 0xEu, stderr);
    result = 1;
  }
  return result;
}

//----- (0000000000402E4C) ----------------------------------------------------
int64_t init_pkey(EVP_MD_CTX *ctx, RSA **pem, EVP_PKEY **ppkey, int isSsh, char isSHA256)
{
  int64_t result; // x0
  const EVP_MD *algo; // x0
  FILE *stream; // [xsp+38h] [xbp+38h]
#ifdef ONDEVICE
  if ( isSsh == 1 )
    stream = fopen("/usr/share/xiaoqiang/public_ssh.pem", "rb");
  else
    stream = fopen("/usr/share/xiaoqiang/public.pem", "rb");
#else
  if ( isSsh == 1 )
    stream = fopen("public_ssh.pem", "rb");
  else
    stream = fopen("public.pem", "rb");
#endif

  if ( stream )
  {
    *pem = PEM_read_RSA_PUBKEY(stream, 0LL, 0LL, 0LL);
    if ( *pem )
    {
      fclose(stream);
      *ppkey = EVP_PKEY_new();
      if ( *ppkey )
      {
        if ( EVP_PKEY_set1_RSA(*ppkey, *pem) == 1 )
        {
          EVP_MD_CTX_init(ctx);
          if(isSHA256)
            algo = EVP_sha256();//for newer model
          else
            algo = EVP_sha1();
          if ( EVP_DigestInit_ex(ctx, algo, 0LL) == 1 )
          {
            result = 0LL;
          }
          else
          {
            fwrite("error EVP_VerfyInit_ex\n", 1u, 0x17u, stderr);
            RSA_free(*pem);
            EVP_PKEY_free(*ppkey);
            result = 0xFFFFFFFFLL;
          }
        }
        else
        {
          fwrite("error EVP_PKEY_st1_RSA\n", 1u, 0x17u, stderr);
          RSA_free(*pem);
          EVP_PKEY_free(*ppkey);
          result = 0xFFFFFFFFLL;
        }
      }
      else
      {
        fwrite("error EVP_PEKY_new\n", 1u, 0x13u, stderr);
        RSA_free(*pem);
        result = 0xFFFFFFFFLL;
      }
    }
    else
    {
      fwrite("error PEM_read_RSAPublicKey\n", 1u, 0x1Cu, stderr);
      fclose(stream);
      result = 0xFFFFFFFFLL;
    }
  }
  else
  {
    fwrite("error fopen public key \n", 1u, 0x18u, stderr);
    result = 0xFFFFFFFFLL;
  }
  return result;
}

//----- (000000000040307C) ----------------------------------------------------
int j_verify_rsa(FILE *f, unsigned int start, unsigned int size, const unsigned char *sigbuf, unsigned int siglen, unsigned int isSsh, char isSHA256)
{
  return verify_rsa(f, start, size, sigbuf, siglen, isSsh, isSHA256);
}

//----- (00000000004030C0) ----------------------------------------------------
int verify_rsa(FILE *f, int start, unsigned int size, const unsigned char *sigbuf, unsigned int siglen, unsigned int isSsh, char isSHA256)
{
  int result; // w0
  EVP_PKEY *pkey; // [xsp+30h] [xbp+30h] BYREF
  RSA *pem; // [xsp+38h] [xbp+38h] BYREF
  EVP_MD_CTX* ctx; // [xsp+40h] [xbp+40h] BYREF
  unsigned int v15; // [xsp+74h] [xbp+74h]
  void *buf; // [xsp+78h] [xbp+78h]
  size_t cnt; // [xsp+84h] [xbp+84h]
  unsigned int v18; // [xsp+88h] [xbp+88h]
  int v19; // [xsp+8Ch] [xbp+8Ch]
  ctx=EVP_MD_CTX_new();
  v18 = size;
  cnt = 0;
  pkey = 0LL;
  buf = malloc(0x10000u);
  if ( buf )
  {
    v19 = init_pkey(ctx, &pem, &pkey, isSsh, isSHA256);
    if ( v19 )
    {
      fwrite("malloc failed\n", 1u, 0xEu, stderr);
      result = 1;
    }
    else
    {
      fseek(f, start, 0);
      while ( v18 )
      {
        v15 = fread(buf, 1u, 0x10000u, f);
        if ( v18 < v15 )
          cnt = v18;
        else
          cnt = v15;
        if ( EVP_DigestUpdate(ctx, buf, cnt) != 1 )// EVP_VerifyUpdate
        {
          fwrite("error EVP_VerifyUpdate\n", 1u, 0x17u, stderr);
          v19 = 1;
          goto finish;
        }
        v18 -= cnt;
        if ( feof(f) && v18 )
        {
          fwrite("File too short while required RSA length too long\n", 1u, 0x32u, stderr);
          v19 = 1;
          goto finish;
        }
      }
      if ( EVP_VerifyFinal(ctx, sigbuf, siglen, pkey) == 1 )
      {
        v19 = 0;
      }
      else
      {
        fwrite("error EVP_VerifyFinal\n", 1u, 0x16u, stderr);
        v19 = 1;
      }
finish:
      rewind(f);
      free(buf);
      RSA_free(pem);
      EVP_PKEY_free(pkey);
      EVP_MD_CTX_free(ctx);
      result = v19;
    }
  }
  else
  {
    fwrite("malloc failed\n", 1u, 0xEu, stderr);
    result = 1;
  }
  return result;
}

//----- (0000000000403318) ----------------------------------------------------
int64_t write_buf(int fd, void *buf, unsigned int size)
{
  signed int i; // [xsp+2Ch] [xbp+2Ch]

  for ( i = 0; i < (int)size; i += write(fd, (char *)buf + i, size - i) )
    ;
  return size;
}

//----- (0000000000403394) ----------------------------------------------------
int open_and_erase_mtd(unsigned int mtdId)
{
  char v3[16]; // [xsp+20h] [xbp+20h] BYREF
  unsigned int i; // [xsp+30h] [xbp+30h] BYREF
  unsigned int v5; // [xsp+34h] [xbp+34h]
  char v6[8]; // [xsp+38h] [xbp+38h] BYREF
  unsigned int v7; // [xsp+40h] [xbp+40h]
  unsigned int v8; // [xsp+44h] [xbp+44h]
  int fd; // [xsp+5Ch] [xbp+5Ch]

  memset(v3, 0, sizeof(v3));
  sprintf(v3, "/dev/mtd%d", mtdId);
  printf("target mtd %s\n", v3);
  fd = open(v3, 2);
  ioctl(fd, 0x80204D01uLL, v6);
  printf("MTD Type: %x\nMTD total size: %x bytes\nMTD erase size: %x bytes\n", (unsigned char)v6[0], v7, v8);
  v5 = v8;
  printf("Beging Eraseing Block %#x\n", i);
  for ( i = 0; i < v7; i += v5 )
  {
    ioctl(fd, 0x40084D06uLL, &i);
    ioctl(fd, 0x40084D02uLL, &i);
  }
  lseek(fd, 0LL, 0);
  puts("End...");
  return fd;
}

//----- (00000000004034BC) ----------------------------------------------------
int64_t calcMd5(unsigned char *md5Output, const char *data)
{
  size_t v2; // w0
  MD5_CTX v6; // [xsp+20h] [xbp+20h] BYREF

  if ( MD5_Init(&v6) )
  {
    v2 = strlen(data);
    if ( MD5_Update(&v6, data, v2) )
    {
      if ( MD5_Final(md5Output, &v6) )
        return 0LL;
      puts("MD5_Final error");
    }
    else
    {
      puts("MD5_Update error");
    }
  }
  else
  {
    puts("MD5_Init error");
  }
  return 0xFFFFFFFFLL;
}

//----- (000000000040355C) ----------------------------------------------------
int64_t transpostGuid(uint8_t *a1, char *outbuf)
{
  char *v2; // x0
  int64_t result; // x0
  char mat[8][32]; // [xsp+28h] [xbp+28h] BYREF
  size_t n; // [xsp+12Ch] [xbp+12Ch]
  char *curRow; // [xsp+130h] [xbp+130h]
  int rowI; // [xsp+138h] [xbp+138h]
  unsigned int row; // [xsp+13Ch] [xbp+13Ch]
  
  row = 0;
  rowI = 0;
  n = 0;
  memset(mat, 0, sizeof(mat));
  while ( *a1 )
  {
    if ( *a1 == '-' )
    {
      mat[row++][rowI] = 0;
      rowI = 0;
    }
    else
    {
      mat[row][rowI++] = *a1;
    }
    ++a1;
  }
  mat[row][rowI] = 0;
  curRow = outbuf;
  while ( 1 )
  {
    result = row;
    if ( (row & 0x80000000) != 0 )
      break;
    n = strlen(mat[row]);
    strncpy(curRow, mat[row], n);
    curRow += (int)n;
    if ( row )
    {
      v2 = curRow++;
      *v2 = '-';
    }
    else
    {
      *curRow = 0;
    }
    --row;
  }
  return result;
}

//----- (00000000004036EC) ----------------------------------------------------
int64_t get_ssh_key()
{
  char magic[100]; // [xsp+EA8h] [xbp+18h] BYREF
  char snStr[100]; // [xsp+F10h] [xbp+80h] BYREF
  char Sn[100]; // [xsp+F78h] [xbp+E8h] BYREF
  unsigned char md5Output[16]; // [xsp+FE0h] [xbp+150h] BYREF
  FILE *stream; // [xsp+FF0h] [xbp+160h]
  int i; // [xsp+FFCh] [xbp+16Ch]

  memset(md5Output, 0, 16);
  memset(Sn, 0, 100);
  memset(snStr, 0, 100);
  memset(magic, 0, 100);
#ifdef ONDEVICE
  stream = popen("nvram get SN", "r");
#else
  stream = fopen("SN.txt", "r");
#endif
  if ( !stream )
    return puts("Failed to run command");

  while ( fgets(Sn, 100, stream) )
    sscanf(Sn, "%s", snStr);

#ifdef ONDEVICE
  pclose(stream);
#else
  fclose(stream);
#endif
  transpostGuid("d44fb0960aa0-a5e6-4a30-250f-6d2df50a", magic);
  sprintf(Sn, "%s%s", snStr, magic);
  calcMd5(md5Output, Sn);
  memset(Sn, 0, sizeof(Sn));
  for ( i = 0; i <= 3; ++i )
    printf("%x%x", (md5Output[i] >> 4) & 0xF, md5Output[i] & 0xF);
  return puts(Sn);
}

//----- (00000000004038A4) ----------------------------------------------------
int extract_ssh_rom_callback(MiRomHdr *hdr, int64_t fileId, MiRomFile *fildhdr, FILE *f, void *SN_str)
{
  int ret; // r4
  unsigned int filesize; // r7
  void *ptr; // r0
  size_t decfileSize; // r4
  FILE *decfileStream; // r5
  FILE *stream; // r4
  char *out; // [sp+8h] [bp-358h] BYREF
  void *dest; // [sp+10h] [bp-350h]
  int outl; // [sp+18h] [bp-348h] BYREF
  int outl1; // [sp+20h] [bp-340h] BYREF
  unsigned char key[16]; // [sp+24h] [bp-33Ch] BYREF
  unsigned char iv[16]; // [sp+34h] [bp-32Ch] BYREF
  char strBuf1[100]; // [sp+44h] [bp-31Ch] BYREF
  char strBuf2[100]; // [sp+A8h] [bp-2B8h] BYREF
  char snStr[100]; // [sp+10Ch] [bp-254h] BYREF
  char guid1[100]; // [sp+170h] [bp-1F0h] BYREF
  char guid2[100]; // [sp+1D4h] [bp-18Ch] BYREF
  char Sn[260]; // [sp+238h] [bp-128h] BYREF
  EVP_CIPHER_CTX *ctx;
  ctx=EVP_CIPHER_CTX_new();

  memset(key, 0, sizeof(key));
  memset(iv, 0, sizeof(iv));
  memset(strBuf1, 0, sizeof(strBuf1));
  memset(strBuf2, 0, sizeof(strBuf2));
  memset(snStr, 0, sizeof(snStr));
  memset(guid1, 0, sizeof(guid1));
  memset(guid2, 0, sizeof(guid2));
  if ( fileId )
    return 1;
  filesize = fildhdr->filesize;
  if ( filesize <= 7 || fildhdr->magic != 0xBABE )
    return 0;
  dest = malloc(fildhdr->filesize);
  if ( dest )
  {
    ptr = malloc(filesize + 1);
    if ( ptr )
    {
      memset(ptr, 0, filesize + 1);
      if ( !SN_str )
      {
        memset(Sn, 0, 100u);
        #ifdef ONDEVICE
          stream = popen("nvram get SN", "r");
        #else
          stream = fopen("SN.txt", "r");
        #endif
        if ( stream )
        {
          (*((uint8_t*)&(fileId))) = (uint8_t)Sn;
          while ( fgets(Sn, 100, stream) )
            sscanf((const char *)Sn, "%s", snStr);
        }
        else
        {
          puts("Failed to run command");
        }
        SN_str = snStr;
        printf("getsn %s\n", snStr);
      }

      transpostGuid("be39095eb72a-b89d-40df-626a-12c5a5c6", guid1);
      transpostGuid("3b1d7f12ba84-ac27-4d18-2aa4-a9cdcbe0", guid2);
      sprintf(strBuf1, "%s%s", SN_str, guid1);
      sprintf(strBuf2, "%s%s", SN_str, guid2);
      printf("key_str:%s\niv_str:%s\n",strBuf1,strBuf2);

      calcMd5(key, strBuf1);
      calcMd5(iv, strBuf2);
      printf("key_md5:");
      for ( char i = 0; i <= 16; ++i ) printf("%02x", key[i]);
      printf("\niv_md5:");
      for ( char i = 0; i < 16; ++i ) printf("%02x", iv[i]);
      printf("\nalgo:aes_128_cbc\n");

      if ( filesize == fread(dest, 1u, filesize, f) )
      {
        out = malloc(fildhdr->filesize);
        EVP_CIPHER_CTX_init(ctx);
        EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), 0, key, iv);
        if ( EVP_DecryptUpdate(ctx, out, &outl, dest, filesize) )
        {
          EVP_DecryptFinal_ex(ctx, out+outl, &outl1);
          memcpy(ptr, out, outl+outl1);
          EVP_CIPHER_CTX_cleanup(ctx);
          decfileSize = outl+outl1;
        }
        else
        {
          decfileSize = -1;
        }
        printf("encfileSize:%d\n",filesize);
        printf("decfileSize:%d\n",decfileSize);
        printf("filename: %s\n",fildhdr->name);
        decfileStream = fopen(fildhdr->name, "w");
        if ( decfileStream )
        {
          if ( !fwrite(ptr, decfileSize, 1u, decfileStream) || fflush(decfileStream) )
          {
            fwrite("fwrite failed\n", 1u, 0xEu, stderr);
            ret = -5;
          }
          else
          {
            ret = 0;
          }
          fclose(decfileStream);
        }
        else
        {
          ret = -2;
          fprintf(stderr, "can not open \"%s\" for writing\n", fildhdr->name);
        }
      }
      else
      {
        ret = -5;
      }
      free(dest);
      free(ptr);
      free(out);
      EVP_CIPHER_CTX_free(ctx);
    }
    else
    {
      fwrite("malloc failed\n", 1u, 0xEu, stderr);
      ret = -12;
      free(dest);
    }
  }
  else
  {
    fwrite("malloc failed\n", 1u, 0xEu, stderr);
    return -12;
  }
  return ret;
}

// nfuncs=134 queued=28 decompiled=28 lumina nreq=0 worse=0 better=0
// ALL OK, 28 function(s) have been successfully decompiled
