//
//  CppBridge.m
//  HP Prime Simulator
//
//  Created by Richie on 17/11/2024.
//

#import <Foundation/Foundation.h>
#import "display.h"

extern double START();

@interface CppBridge : NSObject


@end

@implementation CppBridge {
    
}

- (instancetype)init {
    self = [super init];
    if (self) {
        
    }
    return self;
}

+ (void)initHPPrime {
    memset(getDisplayBufferAddress(), 255, 320 * 240 * 4);
}

+ (double)hpprgm {
    return START();
}


- (void)dealloc {
}

@end

