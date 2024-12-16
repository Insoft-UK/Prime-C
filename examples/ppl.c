//#define MY_DEFINE "Copyright (c) 2023 Insoft. All rights reserved."

real START()
begin
    BLOB blobA = {0x03E07C00007F7FE0,0x007F7FE003E07C00,0x7FFF00007FFF0000,0x00007FFF00007FFF};

    RECT_P(G0, 0, 0, 320, 240, RGB(0, 0, 0, 0));
    DIMGROB_P(G2, 4, 4, blobA);
    BLIT_P(G0, 0, 0, 200, 200, G2, 0, 0, 4, 4);

    DIMGROB_P(G3, 1, 1, RGB(255, 255, 0));
    BLIT_P(G0, 300, 0, 320, 200, G3, 0, 0, 4, 4);

    while(true) {
        SLEEP;
    }

    return 0;
end



