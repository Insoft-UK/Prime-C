//
//  CppBridge.h
//  HP Prime Simulator
//
//  Created by Richie on 17/11/2024.
//

#ifndef CppBridge_h
#define CppBridge_h

#import <Foundation/Foundation.h>

@interface CppBridge : NSObject

+ (void)initHPPrime;
+ (void)hpprgm;

@end

#endif /* CppBridge_h */
