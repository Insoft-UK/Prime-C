//#define MY_DEFINE "Copyright (c) 2023 Insoft. All rights reserved."

enum MyEnum  {
    Red = 0,
    Blue = 1
 };

void enums(void) {
    A = Red;
    B = Blue;
}

void vars(void) {
    var longName;
    var sn;
}

var brach(void) {
    if (A == B || B >= 10) {
        return -1;
    }
    
    if (A >= B) {
        A += 1;
    }
    else {
        A = -1;
    }
}

var loop(void) {
    while (W < 10) {
        W += 1;
    }
    
    do {
        D += 1;
    } while (D < 10);
    
    for (F = 0; F < 10; F += 1) {
        A += 10;
    }
    
    for (; N < 10; N += 1) {
        B += 10;
    }
    
    for (; ; N += 1) {
        B += 10;
        if (B > 100) {
            break;
        }
    }
}


