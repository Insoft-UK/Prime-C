//#define MY_DEFINE "Copyright (c) 2023 Insoft. All rights reserved."

real main()
BEGIN
    real index;
    index[0] = 0;

    for (i=0; i<5; i += 1) {
        // for...loop
    }
    if (index[0]==index[1] && index[0] <= 0) {
        return 1;
    } else {
        return 0;
    }

    integer counterA=400;
    while (counterA > 0) {
        counterA-=1;
    }

    integer counterB = 100;
    repeat {
        counterB -= 10;
    } until(counterB <= 0);

    return 0;
END


