/* UT El Paso EE 4374 Lab 4
   Socket Programming Client/Server application
   Author: Jose Caballero
   04/26/2016 */ 

/* Bank Transaction Types */
#define BANK_TRANS_DEPOSIT    0
#define BANK_TRANS_WITHDRAW   1
#define BANK_TRANS_INQUIRY    2

typedef struct
{
    unsigned int trans;		/* transaction types */
    unsigned int acctnum;	/* account number */
    unsigned int value;		/* value */
} sBANK_PROTOCOL;

extern sBANK_PROTOCOL bankData;
