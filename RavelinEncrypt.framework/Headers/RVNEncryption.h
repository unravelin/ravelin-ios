//
//  RVNEncryption.h
//  RavelinCore
//
//  Created by David B on 12/10/2018.
//  Copyright © 2018 Ravelin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonCrypto.h>
NS_ASSUME_NONNULL_BEGIN

// Encryption modes
extern const struct RVNEncryptionModeReadable
{
    __unsafe_unretained NSString *RSAOAEP_WITH_AES_256_GCM;
} RVNEncryptionMode;

/*! Chilkat serial number */
extern NSString *const kRVNChilkat;

@interface RVNEncryption : NSObject

// Error constants
typedef enum : int {
    RVNEncryptionInvalidKeyLengthError = 1000,
    RVNEncryptionInvalidHexKeyError,
    RVNEncryptionRSAKeyMissingError,
    RVNEncryptionKeyImportError,
    RVNEncryptionInitError,
    RVNEncryptionStringEncryptError,
    RVNEncryptionRSAStringEncryptError,
    RVNEncryptionPanInvalidError,
    RVNEncryptionMonthInvalidError,
    RVNEncryptionYearInvalidError
} RVNErrors;

/**
 Get the instantiated Ravelin singleton
 @return The singleton instance of the class
 - Remark: You will need to set rsaKey before encrpytion
 */
+ (instancetype)sharedInstance;

/**
 Generates encryption payload ready for sending to Ravelin
 @param pan A string representation of the long card number
 @param month Expiry month of card (1-12)
 @param year Expiry year (2 or 4 digit)
 @param nameOnCard The customer name on the card
 @param error Passed as reference
 @return Dictionary containing methodType, aesKeyCiphertext, cardCiphertext, algorithm, keyIndex and ravelinSDKVersion
 */
- (NSDictionary *)encrypt:(NSString *)pan month:(NSString *)month year:(NSString *)year nameOnCard:(NSString *)nameOnCard error:(NSError **)error;

/** The public rsa key from your dashboard */
@property(nonatomic, strong) NSString *_Nullable rsaKey;

@end

NS_ASSUME_NONNULL_END
