//
//  Ravelin.h
//  RavelinCore
//
//  Created by David Brown on 04/10/2018.
//  Copyright © 2018 Ravelin. All rights reserved.
//

#import <Foundation/Foundation.h>


NS_ASSUME_NONNULL_BEGIN


/**
 Representation of the main Ravelin Core Class
 */
@interface Ravelin : NSObject

/**
 Create a singleton instance of the Ravelin sdk with your public key
 @param apiKey The public API key from your Ravelin dashboard
 @return The singleton instance of the class
 - Remark: Use this method when using Ravelin in your app for the first time
 */
+ (instancetype)createInstance:(NSString *)apiKey;

/**
 Create a singleton instance of the Ravelin sdk with your public key
 @param apiKey The public API key from your Ravelin dashboard
 @param rsaKey The public RSA key from your Ravelin dashboard
 @return The singleton instance of the class
 - Remark: This method is now deprecated, as encryption is now handled in the RavelinEncrypt framework. Use createInstance:apiKey instead.
 */
+ (instancetype)createInstance:(NSString *)apiKey rsaKey:(NSString *)rsaKey __deprecated_msg("Use createInstance:apiKey instead - encryption is now handled in the RavelinEncrypt framework");

/**
 Get the instantiated Ravelin singleton
 @return The singleton instance of the class
 - Remark: You will need to set apiKey before use
 */
+ (instancetype)sharedInstance;

/** The public api key from your dashboard */
@property(nonatomic, strong) NSString *apiKey;

/** Your chosen customer id */
@property(nonatomic, strong) NSString *customerId;

/** Temp customer id */
@property(nonatomic, strong) NSString *tempCustomerId;

/** The Ravelin generated sessionId */
@property(nonatomic, strong, readonly) NSString *sessionId;

/** The Ravelin generated device id */
@property(nonatomic, strong, readonly, getter=getDeviceId) NSString *deviceId;

/** Your chosen order id */
@property(nonatomic, strong) NSString *orderId;

/**
 Fingerprints the device and sends results to Ravelin
 */
- (void)trackFingerprint;

/**
 Fingerprints the device and sends results to Ravelin
 @param completionHandler Completion block to handle response
 */
- (void)trackFingerprint:(void (^)(NSData *_Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

/**
 Fingerprints the device and sends results to Ravelin
 @param customerId The customerId to set for this device fingerprint.
 @param completionHandler Completion block to handle response
 */
- (void)trackFingerprint:(NSString *)customerId completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

/**
 Sends a track event to Ravelin
 @param pageTitle The title of the current page
 @param eventName The name of the event
 @param eventProperties A dictionary of meta data to send with the event
 @code
 NSDictionary *meta = @{@"SEARCHED_PRODUCT" : @"ProductName"};
 [self track:@"CUSTOM_EVENT" eventProperties:meta];
 @endcode
 */
- (void)track:(NSString * _Nullable)pageTitle eventName:(NSString *)eventName eventProperties:(NSDictionary * _Nullable)eventProperties;

/**
 Sends a track event to Ravelin
 @param pageTitle The title of the current page
 @param eventName The name of the event
 @param eventProperties A dictionary of meta data to send with the event
 @param completionHandler Completion block to handle response
 */
- (void)track:(NSString * _Nullable)pageTitle eventName:(NSString *)eventName eventProperties:(NSDictionary * _Nullable)eventProperties completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

/**
 Sends a track page event to Ravelin
 @param pageTitle The title of the current page
 */
- (void)trackPage:(NSString * _Nullable)pageTitle;

/**
 Sends a track page event to Ravelin
 @param pageTitle The title of the current page
 @param eventProperties A dictionary of meta data to send with the event
 */
- (void)trackPage:(NSString * _Nullable)pageTitle eventProperties:(NSDictionary * _Nullable)eventProperties;

/**
 Sends a track page event to Ravelin
 @param pageTitle The title of the current page
 @param eventProperties A dictionary of meta data to send with the event
 @param completionHandler Completion block to handle response
 */
- (void)trackPage:(NSString * _Nullable)pageTitle eventProperties:(NSDictionary * _Nullable)eventProperties completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

/**
 Sends a track login event to Ravelin and sends customerId automatically if set
 @param pageTitle The title of the current page
 */
- (void)trackLogin:(NSString * _Nullable)pageTitle;

/**
 Sends a track login event to Ravelin
 @param pageTitle The title of the current page
 @param eventProperties A dictionary of meta data to send with the event
 */
- (void)trackLogin:(NSString * _Nullable)pageTitle eventProperties:(NSDictionary * _Nullable)eventProperties;

/**
 Sends a track login event to Ravelin
 @param pageTitle The title of the current page
 @param eventProperties A dictionary of meta data to send with the event
 @param completionHandler Completion block to handle response
 */
- (void)trackLogin:(NSString * _Nullable)pageTitle eventProperties:(NSDictionary * _Nullable)eventProperties completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

/**
 Ends current Ravelin session and sends logout event to Ravelin
 @param pageTitle The title of the current page
 */
- (void)trackLogout:(NSString * _Nullable)pageTitle;

/**
 Ends current Ravelin session and sends logout event to Ravelin
 @param pageTitle The title of the current page
 @param eventProperties A dictionary of meta data to send with the event
 */
- (void)trackLogout:(NSString * _Nullable)pageTitle eventProperties:(NSDictionary * _Nullable)eventProperties;

/**
 Sends a track logout event to Ravelin
 @param pageTitle The title of the current page
 @param eventProperties A dictionary of meta data to send with the event
 @param completionHandler Completion block to handle response
 */
- (void)trackLogout:(NSString * _Nullable)pageTitle eventProperties:(NSDictionary * _Nullable)eventProperties completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

// Force singleton use by making init and new unavailable
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
