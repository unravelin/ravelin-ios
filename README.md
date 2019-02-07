# Getting Started

The Mobile SDK provides a means of adding customer information to Ravelin without having to handle PCI-compliant data. The library is intended to work in your apps where you have access to card details the customer has entered. Our SDK will encrypt sensitive data so it can pass through your servers to Ravelin and  reduce the scope of your PCI compliance.

This is all achieved using our library which handles client-side encryption, as well as our device and session tracking functionality.

## Contents

* [Installation](#installing-the-ravelin-mobile-sdk-via-cocoapods)
* [Usage](#usage)
* [Examples](#end-to-end-example)
* [Class Reference](#ravelin-class-reference)
* [Licemse](#license)

## Building the SDK from source

If you wish to build the framework from source, the source code repository uses Git LFS (large file storage) to store some files. Please install Git-LFS before cloning locally. Follow the instructions on the [Git LFS website](https://git-lfs.github.com/) to set LFS up.

## Installing the Ravelin Mobile SDK via Cocoapods

Add `pod 'RavelinCore'` to your PodFile then from the command line `pod install`

## Installing the Ravelin Mobile SDK via Carthage

Add the following line to your Cartfile

`github "unravelin/ravelin-ios" == 0.1.1`

Then from the command line 

`carthage update`

## Installing the Ravelin Mobile SDK (manually)

The Ravelin Mobile SDK is provided as a precompiled Cocoa Touch framework.

To manually install:

1. Open your project or xcworkspace

2. Select the Product target or the xcodeproj and goto the “General” tab

3. Find or search for “Embedded Binaries” and drag and drop the RavelinCore.framework file to it. At the prompt, select “copy items if needed”
![alt text](docs/images/ravelin-install-copy-items-prompt.png)

4. The framework should now be shown in Embedded Binaries and Linked Frameworks and Libraries. If you do not see it in Linked Frameworks and Libraries, repeat step 3 for this section also.
![alt text](docs/images/ravelin-install-embedded-binaries.png)

### Preparing for the App Store (when installed manually)

> The framework is a "fat binary" and contains an x86_64 build for the Xcode simulator along with device builds. At the time of writing, if you have manually installed the framework and are using Xcode to send your app to the App Store, you will need to add the script below in build phases to 'thin out' the binary to remove the simulator architectures.  This is achieved by selecting your project and going to Build Phases and using + to add a new "Run Script Phase".

> ![script phases](docs/images/ravelin-build-phases-run-script.png)


```swift

# This script loops through the frameworks embedded in the application and
# removes unused architectures.
# See https://stackoverflow.com/questions/46404236/failed-to-verify-bitcode-while-exporting-archive-for-ad-hoc-distribution-tried

APP_PATH="${TARGET_BUILD_DIR}/${WRAPPER_NAME}"

find "$APP_PATH" -name '*.framework' -type d | while read -r FRAMEWORK
do
FRAMEWORK_EXECUTABLE_NAME=$(defaults read "$FRAMEWORK/Info.plist" CFBundleExecutable)
FRAMEWORK_EXECUTABLE_PATH="$FRAMEWORK/$FRAMEWORK_EXECUTABLE_NAME"
echo "Executable is $FRAMEWORK_EXECUTABLE_PATH"

EXTRACTED_ARCHS=()

for ARCH in $ARCHS
do
echo "Extracting $ARCH from $FRAMEWORK_EXECUTABLE_NAME"
lipo -extract "$ARCH" "$FRAMEWORK_EXECUTABLE_PATH" -o "$FRAMEWORK_EXECUTABLE_PATH-$ARCH"
EXTRACTED_ARCHS+=("$FRAMEWORK_EXECUTABLE_PATH-$ARCH")
done

echo "Merging extracted architectures: ${ARCHS}"
lipo -o "$FRAMEWORK_EXECUTABLE_PATH-merged" -create "${EXTRACTED_ARCHS[@]}"
rm "${EXTRACTED_ARCHS[@]}"

echo "Replacing original executable with thinned version"
rm "$FRAMEWORK_EXECUTABLE_PATH"
mv "$FRAMEWORK_EXECUTABLE_PATH-merged" "$FRAMEWORK_EXECUTABLE_PATH"

done
```

## Usage

To use the framework within your project, import RavelinCore where required:

#### Objective-C
```objc
#import <RavelinCore/Ravelin.h>
```

#### Swift
```swift
import RavelinCore
```

The singleton Ravelin class should be accessed via the sharedInstance method. You will also need to initialise the SDK with a sharedInstanceWithApiKey method call to set your Ravelin public API and RSA keys:

#### Objective-C
```objc
self.ravelin = [Ravelin sharedInstanceWithApiKey:@"publishable_key_live_----" rsaKey:@"----|----"];
```

#### Swift
```swift
let ravelin = Ravelin.sharedInstance(withApiKey: "publishable_key_live_----", rsaKey: "----|----")
```

Once initialised, you can use the sharedInstance directly to access methods and properties

#### Objective-C
```objc

// Directly
[[Ravelin sharedInstance] methodName];

// Variable
Ravelin *ravelin = [Ravelin sharedInstance];

```

#### Swift
```swift

// Directly
Ravelin.sharedInstance().methodName()

// Variable
let ravelin = Ravelin.sharedInstance()
```

## Encrypting Cards

The primary goal of the Ravelin Mobile SDK is to securely send encrypted card information to Ravelin without handling PCI-compliant data. When collecting the card details, we encrypt the values to send using the code method below. Validation is performed, confirming that expiry dates are valid and that the PAN is at least 13 characters. Should any validation checks fail, nil is returned from the method. Pass an error by ref to determine the cause of failure if any occurs.

#### Objective-C
```objc
// Card details
NSString *pan = @"41111111111111";
NSString *month = @"10";
NSString *year = @"2022";
NSString *cardHolder = @"Mr John Doe";

// Error handling
NSError *error;

// Encrypt
NSDictionary *encryptionPayload = [[Ravelin sharedInstance] encrypt:pan month:month year:year nameOnCard:cardHolder error:&error];

if(!error) {
    NSLog(@"Ravelin encryption payload: %@",encryptionPayload);
    // Send to your servers

} else {
    NSLog(@"Ravelin encryption error %@", error.localizedDescription);
}
```

#### Swift
```swift
var error:NSError? = nil

let encryptionPayload = Ravelin.sharedInstance().encrypt("41111111111111", month: "10", year: "10", nameOnCard: "Mr John Doe", error: &error)

if let error = error {
    print("Ravelin encryption error \(error.localizedDescription)")
} else {
    print("Ravelin Encryption payload: \(encryptionPayload as AnyObject)")
    // Send to your servers
}
```

## Tracking Activity

Using the Ravelin Mobile SDK, you can capture various built in events along with your own custom ones that can later be viewed in the Ravelin Dashboard.

> __Ravelin.trackPage:(NSString *)pageTitle__ - to indicate when the user hits a new page. 

> __Ravelin.trackLogin:(NSString *)pageTitle eventProperties:(NSDictionary *)eventProperties__ - to indicate that the user has just authenticated themselves. Use eventProperties to add additional key/pair information to the payload

> __Ravelin.trackLogout:(NSString *)pageTitle eventProperties:(NSDictionary *)eventProperties__ - to indicate when the user has signed out of their session.

> __Ravelin.fingerprint:(NSDictionary *)eventProperties__ – To be used at checkout to profile the users device.

## Custom Events and Metadata

The track method can be used to log notable client-side events:

#### Objective-C
```objc
NSString *pagetitle = @"products";
NSString *eventName = @"PRODUCT_SEARCH";
NSDictionary *meta = @{@"productId" : @"213", @"sizeId" : @"M"};
[[Ravelin sharedInstance]track:pageTitle eventName:eventName eventProperties:meta];
```

#### Swift
```swift
let pageTitle = "products"
let eventName = "PRODUCT_SEARCH"
let meta = ["productId" : "213", "SIZE" : "M"]
Ravelin.sharedInstance().track(pageTitle, eventName: eventName, eventProperties: meta)
```

> __NOTE:__ Track events have overload methods with completiton handlers

## Fingerprint location tracking

> For location tracking to be successful from within the Ravelin Mobile SDK, your application should ask for user permissions for location sharing. Please refer to the Apple documentation [here](https://developer.apple.com/documentation/corelocation) for more information on the subject.


## End to end example

What follows is a simple end-to-end example of using the Ravelin Framework within a View.

> __NOTE:__ All Ravelin network methods are asynchronous. Completion blocks are provided so you can handle each request accordingly. The example code will not necessarily call each method sequentially and is for demonstration purposes only.


#### Objective-C
```objc
#import "ViewController.h"
#import <UIKit/UIKit.h>
#import <RavelinCore/Ravelin.h>
@interface ViewController ()
@property (strong, nonatomic) Ravelin *ravelin;
@end

@implementation ViewController


- (void)viewDidLoad {
    [super viewDidLoad];

    
    // Make Ravelin instance with api keys
    self.ravelin = [Ravelin sharedInstanceWithApiKey:@"publishable_key_live_----" rsaKey:@"----|----"];
    
    // Setup customer info and track their login
    self.ravelin.customerId = @"customer1234";
    self.ravelin.orderId = @"web-001";
    [self.ravelin trackLogin:@"loginPage"];
    
    // Track customer moving to a new page
    [self.ravelin trackPage:@"checkout"];
    
    // Send a device fingerprint
    [self.ravelin fingerprint];
    
    // Send a device fingerprint with a completion block (if required)
    [self.ravelin fingerprint:^(NSData *data, NSURLResponse *response, NSError *error) {
        if(!error) {
            NSDictionary *responseData;
            NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
            if (httpResponse.statusCode == 200) {
                responseData = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers|NSJSONReadingAllowFragments error:nil];
                // Do something with responseData
                
            } else {
                // Status was not 200. Handle failure
            }
        } else {
            NSLog(@"%@",error.localizedDescription);
        }
    }];
    
    // Encrypt customer card details, ready for sending for payment
    NSError *error;
    NSDictionary *encryptionPayload = [self.ravelin encrypt:@"41111111111111" month:@"10" year:@"20" nameOnCard:@"Mr John Doe" error:&error];
    if(!error) {
        NSLog(@"Ravelin Encryption payload: %@", encryptionPayload);
    } else {
        NSLog(@"Ravelin encryption error %@", error.localizedDescription);
    }
    
    // Track a customer logout
    [self.ravelin trackLogout:@"logoutPage"];
}
@end
```

#### Swift

```swift
import UIKit
import RavelinCore

class ViewController: UIViewController {

    // Declare Ravelin Shared Instance with API keys
    private var ravelin : Ravelin = Ravelin.sharedInstance(withApiKey: "publishable_key_live_----", rsaKey: "----|----")
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Setup customer info and track their login
        ravelin.customerId = "customer1234"
        ravelin.orderId = "web-001"
        ravelin.trackLogin("loginPage")
        
        // Track customer moving to a new page
        ravelin.trackPage("checkout")
        
        // Send a device fingerprint
        ravelin.fingerprint()
        
        // Send a device fingerprint with a completion block (if required)
        ravelin.fingerprint { (data:Data, response:URLResponse, error:Error?) -> Void in
            if let error = error {
                print(error)
                return
            }
            if let httpResponse = response as? HTTPURLResponse {
                if httpResponse.statusCode == 200 {
                    do {
                        let responseData = try JSONSerialization.jsonObject(with: data, options: [.allowFragments,.mutableContainers])
                        
                        // Do something with responseData
                    } catch {
                        print(error)
                    }
                    
                } else {
                    // Status was not 200. Handle failure
                    print("Failed")
                }
            }
        }
        
        // Encrypt customer card details, ready for sending for payment
        var error:NSError? = nil
        let encryptionPayload = ravelin.encrypt("41111111111111", month: "10", year: "20", nameOnCard: "Mr John Doe", error: &error)
        if let error = error {
            print("Ravelin encryption error \(error.localizedDescription)")
        } else {
            print("Ravelin Encryption payload: \(encryptionPayload as AnyObject)")
            // Send to server
        }
        
        // Track a customer logout
        ravelin.trackLogout("logoutPage")

    }
}
```

# Ravelin Class Reference

## Class Methods

---
## sharedInstanceWithApiKey (apiKey, rsaKey)

Create a singleton instance of the Ravelin sdk with your public keys

### Parameters

| Parameter     | Type               | Description  |
| ------------- |---------------------|-------|
| apiKey     | String     | The public api key from your Ravelin dashboard |
| rsaKey     | String     | The public RSA key from your Ravelin dashboard |

### Return value

> The singleton instance of the class

---

## sharedInstance

Get the instantiated Ravelin singleton


### Return value

> The singleton instance of the class

---

## fingerprint

Fingerprints the device and sends results to Ravelin

---

## fingerprint (completionHandler)

Fingerprints the device and sends results to Ravelin

### Parameters

| Parameter     | Type               | Description  |
| ------------- |---------------------|-------|
| completionHandler     | Object     | Completion block to handle response |

---

## fingerprint(customerId, completionHandler)

Fingerprints the device and sends results to Ravelin

### Parameters

| Parameter     | Type               | Description  |
| ------------- |---------------------|-------|
| customerId     | String     | The customerId to set for this device fingerprint. |
| completionHandler     | Object     | Completion block to handle response |

---

## generateFingerprintPayload
Creates a complete device fingerprint payload (as used by the fingerPrint method)
> **NOTES** Included for testing purposes. The fingerprint method uses this internally.


### Return value

> Dictionary containing the complete payload to send

---

## encrypt (pan, month, year, nameOnCard, &error)

Generates encryption payload ready for sending to Ravelin

### Parameters

| Parameter     | Type               | Description  |
| ------------- |---------------------|-------|
| pan     | String     | A string representation of the long card number |
| month     | String     | Expiry month of card (1-12) |
| year     | String     | Expiry year (2 or 4 digit) |
| nameOnCard     | String     | The customer name on the card |
| error     | Object     | Passed as reference |

### Return value

> Dictionary containing methodType, aesKeyCiphertext, cardCiphertext, algorithm, keyIndex and ravelinSDKVersion

---

## encrypt (pan, month, year, nameOnCard, &error)

Generates encryption payload ready for sending to Ravelin

### Parameters

| Parameter     | Type               | Description  |
| ------------- |---------------------|-------|
| pan     | String     | A string representation of the long card number |
| month     | String     | Expiry month of card (1-12) |
| year     | String     | Expiry year (2 or 4 digit) |
| nameOnCard     | String     | The customer name on the card |
| error     | Object     | Passed as reference |

### Return value

> Dictionary containing methodType, aesKeyCiphertext, cardCiphertext, algorithm, keyIndex and ravelinSDKVersion

---

## track (pageTitle, eventName, eventProperties)

Sends a track event to Ravelin. Use this method to send custom events and data to analyse in your dashboard.

### Parameters

| Parameter           | Type            | Description  |
| --------------------|-----------------|----------------------------|
| pageTitle           | String          | The title of the current page |
| eventName           | String          | The name of the event |
| eventProperties     | Dictionary      | A dictionary of meta data to send with the event |

> Also available with a completion handler: track (pageTitle, eventName, eventProperties, completionHandler)

---

## trackPage (pageTitle, eventProperties)

Sends a track page event to Ravelin.

### Parameters

| Parameter           | Type            | Description  |
| --------------------|-----------------|----------------------------|
| pageTitle           | String          | The title of the current page |
| eventProperties     | Dictionary      | A dictionary of meta data to send with the event |

> Also available with a completion handler: trackPage (pageTitle, eventProperties, completionHandler)


---


## trackLogin (pageTitle, eventProperties)

Sends a track login event to Ravelin

### Parameters

| Parameter           | Type            | Description  |
| --------------------|-----------------|----------------------------|
| pageTitle           | String          | The title of the current page |
| eventProperties     | Dictionary      | A dictionary of meta data to send with the event |

> Also available with a completion handler: trackLogin (pageTitle, eventProperties, completionHandler)


---

## trackLogout (pageTitle, eventProperties)

Ends current Ravelin session and sends logout event to Ravelin

### Parameters

| Parameter           | Type            | Description  |
| --------------------|-----------------|----------------------------|
| pageTitle           | String          | The title of the current page |
| eventProperties     | Dictionary      | A dictionary of meta data to send with the event |

> Also available with a completion handler: trackLogout (pageTitle, eventProperties, completionHandler)


---

## Class Properties

---

## apiKey

> The public api key from your dashboard 


## rsaKey

> The public rsa key from your dashboard


## customerId

> Your chosen customer id 


## tempCustomerId

> Temp customer id  


## sessionId (read only)

> The Ravelin generated sessionId


## deviceId (read only)

> The Ravelin generated device id 


## orderId

> Your chosen order id

# License

License information can be found [here](LICENSE)
