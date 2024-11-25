//
//  CppBridge.m
//  HP Prime Simulator
//
//  Created by Richie on 17/11/2024.
//

#import <Foundation/Foundation.h>
#import "prime.hpp"

extern SUB START();

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

+ (void)hpprgm {
    return START();
}


- (void)dealloc {
}

@end
