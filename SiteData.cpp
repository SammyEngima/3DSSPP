//This file contains C_Hom_App helper member functions for configuring the
//application.

#include "stdafx.h"
#include "hom.h"
#include "time.h"
#include "AppVersion.hpp"
#include "wincrypt.h"       //For hashing

// Valid 440 keys
// Note: keep these as valid keys as long as the
//		upgrades are free.  When we make a paid
//		upgrade, remove these keys from the keychain
//		and make a completely new set.  This is so
//		that customers who make minor upgrades don't
//		have to put in new reg codes every time.

// In addition: Use only letters, not numbers, except
//		in the "2D440" or "3D500" to avoid confusion
//		between, 1 and I, 0 and O, etc.  Also avoid O's and I's.

//New 5.0.0 hashed keys for more secure registration. These are the same
//as the keys below but are saved in the hashed version instead of plaintext
const char* keys_v500[] = {
   { "cc8eb5dcd2a684b1ac2216aa4468f792" },   // Key #4.40.1
   { "ec104e22f4ad9acaccf46aad3fa71144" },   // Key #4.40.2
   { "a49e9fa66fe6588d1a665601241936c3" },   // Key #4.40.3
   { "674b9be84a68017452f6e9c8de26d06b" },   // Key #4.40.4
   { "e5dd0042831e3e5c11c1ae666c505591" },   // Key #4.40.5
   { "5eb3376b63e95cb9681d5e64040f9e1a" },   // Key #4.40.6
   { "032c1f011fb342cbce6707bb59bd75fc" },   // Key #4.40.7
   { "55e933a5a7e63860748437afb8ac51df" },   // Key #4.40.8
   { "b57cb20c42770278118e7428231a07be" },   // Key #4.40.9
   { "135e1e7a3396f68bf683adfa3a502cdf" },   // Key #4.40.10
   { "80750ace3746eb7e484edca0e4e778e9" },   // Key #4.40.11
   { "d670a2d73126033a5c93a03ade225dd3" },   // Key #4.40.12
   { "a484aa108aa319e0be6c6ee98686c3f6" },   // Key #4.40.13
   { "ff51b74c5b7904f99e92cf3100e3b1a9" },   // Key #4.40.14
   { "faa6e87763f802b61a9e41c2ceb95740" },   // Key #4.40.15
   
   { "0d4e427d47810d814894aefe23b0555c" },   // Key #4.40.16 (2D ONLY)

   { "9d37a27ae57236b833f7e7f2ac3cbb14" },   // Key #4.40.UM - UMich ONLY key
   NULL
};
// All 440's changed to 500 for 5.0.0 version
//const char* keys_v500[] = {
//   { "JNBJL-3D500-XESHB-WEWMZ" },   // Key #4.40.1
//   { "KZKXC-3D500-NVNLA-KDJFH" },   // Key #4.40.2
//   { "WEUQH-3D500-QEDDH-UVABN" },   // Key #4.40.3
//   { "TPDJK-3D500-CBNAL-EXCJQ" },   // Key #4.40.4
//   { "NMCJH-3D500-NMAVF-EVBND" },   // Key #4.40.5
//   { "BFKJB-3D500-PEBFQ-USHST" },   // Key #4.40.6
//   { "VTBTH-3D500-UKJEK-RMNSH" },   // Key #4.40.7
//   { "JSGDG-3D500-DFCVX-LSPGB" },   // Key #4.40.8
//   { "NUUNK-3D500-FGJVN-DJSGC" },   // Key #4.40.9
//   { "YUXDN-3D500-FVPBE-DRWWJ" },   // Key #4.40.10
//   { "NDJVC-3D500-EURYT-HGNSX" },   // Key #4.40.11
//   { "EFNWV-3D500-BSMUF-HSBSX" },   // Key #4.40.12
//   { "GCTQZ-3D500-RSUFN-UMMPB" },   // Key #4.40.13
//   { "EHLUK-3D500-MTNFG-SHSUU" },   // Key #4.40.14
//   { "CYDJK-3D500-HSETN-BKWNY" },   // Key #4.40.15
//   
//   { "VVZUE-2D500-BJHBK-NJDJP" },   // Key #4.40.16 (2D ONLY)
//
//   { "ALPHA-3D500-GAMMA-LAMDA" },   // Key #4.40.UM - UMich ONLY key
//   NULL
//};

// Obsolete codes - these remain for reference only.
//const char* keys_v436[] = {
//	{ "DEF35-FF436-224F7-D1810" }, // Key #4.34.1  - Month1
//	{ "47C8A-FF436-6401B-3CBFA" }, // Key #4.34.2  - Month2
//	{ "3119E-FF436-6C605-4E780" }, // Key #4.34.3  - Month3
//	{ "2B936-FF436-76FCF-62643" }, // Key #4.34.4  - Month4
//	{ "14053-FF436-1A94C-8C72B" }, // Key #4.34.5  - Month5
//	{ "9B950-FF436-A8EEA-65A95" }, // Key #4.34.6  - Month6
//	{ "2EA91-FF436-17B50-CBD43" }, // Key #4.34.7  - Month7
//	{ "44744-FF436-4775B-858CC" }, // Key #4.34.8  - Month8
//	{ "FDCC2-FF436-67F71-7D7FD" }, // Key #4.34.9  - Month9
//	{ "A2A3F-FF436-539C0-0A4DC" }, // Key #4.34.10 - Month10
//	{ "8CA8A-FF436-137CA-EA518" }, // Key #4.34.11 - Month11
//	{ "1C5F8-FF436-7D006-DE7D8" }, // Key #4.34.12 - Month12
//	{ "519EB-FE436-A5D72-D9E8F" }, // Key #4.34.13 - 2D mode (Denoted by "FE" in 2nd block)
//	{ "E0C1D-01436-632D0-3BE1A" }, // Key #4.34.14 - 
//	{ "7D6AC-02436-CDF8A-42246" }, // Key #4.34.15 - 
//	{ "52E92-04436-94F2E-5B3B0" }, // Key #4.34.16 - 
//	{ "ED353-08436-1AEB6-50E42" }, // Key #4.34.17 - 
//	{ "5829C-10436-CAD89-3350C" }, // Key #4.34.18 - 
//	{ "985AA-20436-F720D-60E89" }, // Key #4.34.19 - 
//	{ "3B8C0-40436-0DC80-A0748" }, // Key #4.34.20 - 
//	{ "ALPHA-FF436-GAMMA-LAMDA" }, // Key #4.34.21 - UMich ONLY key
//	NULL
//};

// Keyring == keys allowed in this build
//	(Should be all keys since the last paid build - customers should not
//		have to type in a new registration code unless they're purchasing
//		a new version.
const char** keyring[] = {
    keys_v500,
    NULL	// Make sure to keep this here, or we'll get a buffer overrun.
};

// Uses szRegKey passed in, or else if no key
//    is passed in then it will try to read the
//    registration key from the registry.
BOOL C_Hom_App::SiteData(CString szRegKey)
{
   int Success = FALSE;

   if(szRegKey.GetLength() == 0)
      szRegKey  = GetProfileString("Registration", "RegKey" , "none");

   bool validKey = false;
   int keyset, key;
   CString hashedKey = "";
   if (!CreateMD5Hash(szRegKey, hashedKey))
   {
       AfxMessageBox("Unable to verify registration key. Please try again");
       return Success;
   }
   for (keyset = 0; keyring[keyset] != NULL; ++keyset) // For each KEYSET on KEYRING
      for (key = 0; keyring[keyset][key] != NULL; ++key)  // For each KEY in KEYSET
      {
        if (strcmp(LPCTSTR(hashedKey), keyring[keyset][key]) == 0)
	    {
	        validKey = true;
	        this->Release_Type = "Uncrippled";
	        Success = TRUE;

	        // If the 2D code is set, set the
	        //		program to 2D-only.
	        if(szRegKey.Find("2D") != -1)
			    m2DModeOnly = true;

	        break;
	    }
      }

   if (!validKey)
   {
      Success = FALSE;
      this->Release_Type = "Demonstration";	// Default to demo mode
//      this->Company_Name = "Demo Mode Operation.";
   }

   return Success;
}

bool C_Hom_App::CreateMD5Hash(CString &plaintext, CString &hashed)
{
    //Variables for CryptoAPI and hashing
    HCRYPTPROV hProv;   
    HCRYPTHASH hHash;
    DWORD      dwInputLength = plaintext.GetLength();
    BYTE*      bInput = (BYTE*)plaintext.GetBuffer(dwInputLength);
    BYTE       bHash[0x7F];
    DWORD      dwHashLength = sizeof(DWORD);

    //Attempt to acquire a cryptographic context and create a hash object. If both of these succeed,
    //hash 'plaintext' using an MD5 hash.
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
    {
        plaintext.ReleaseBuffer();
        return false;
    }
    
    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        plaintext.ReleaseBuffer();
        CryptReleaseContext(hProv, 0);
        return false;
    }
            
    if (CryptHashData(hHash, bInput, dwInputLength, 0))
    {
        //The data has been successfully hashed at this point. Now we need to get the actual
        //hash and convert it to a string.
        BYTE *pbHashSize = (BYTE*)malloc(dwHashLength);
        if (CryptGetHashParam(hHash, HP_HASHSIZE, pbHashSize, &dwHashLength, 0))
        {
            dwHashLength = (DWORD)*pbHashSize;
            if (CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLength, 0))
            {
                hashed.Empty();
                CString tmp;
                for (int i = 0; i < (int)dwHashLength; ++i)
                {
                    tmp.Format("%02x", bHash[i]);
                    hashed += tmp;
                }
            }
        }
        free(pbHashSize);
    }

    CryptDestroyHash(hHash); 
    CryptReleaseContext(hProv, 0); 
	plaintext.ReleaseBuffer();

    if (hashed.IsEmpty())
        return false;
    
    return true;
}
