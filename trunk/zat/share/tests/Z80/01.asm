        ORG     2400h
	INCLUDE "Z80.inc"
TEST    EQU     INTSP-$
	LD      BC,#1234
INIT    LD      HL,#8000
        JP      INITROU
	JP      INIT
PLAY    SUB     A
        LD      (ENSAVE+1),A
	JR	PLAY
        LD      (MIXER),A
        LD      HL,CURSPD
        DEC     (HL)
        JP      NZ,SDRIVE

SETREG0 EQU     SETREG2+ENSAVE
HEX     equ     1
GLOBSPD         LD      (HL),0
        LD      B,A
        CALL    ACHAN
        CALL    BCHAN
        CALL    CCHAN
SDRIVE  CALL    ADVSAMP
        CALL    BDVSAMP
        CALL    CDVSAMP
ENSAVE  LD      A,0
ENSAVE2         AND     A
        LD      A,12
        LD      C,#FD
        LD      HL,HIHENV
        LD      DE,#FFBF
        JP      Z,SETREG2
        INC     A
        INC     HL
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
SETREG2         LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        DEC     A
        LD      B,D
        OUT     (C),A
        LD      B,E
        OUTD
        RET
ASAMPSL         SUB     #60
ASAMPD  LD      HL,0
        ADD     A,A
        ADD     A,A
        LD      C,A
        ADD     HL,BC
        LD      A,(HL)
        LD      (AREPOS+1),A
        INC     HL
        LD      A,(HL)
        LD      (ASMPLEN+1),A
        INC     HL
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        LD      (ASAMPLE+1),HL
        JP      ADECOM
AORNSL  SUB     #71
        JP      M,AZORN
AORNMD  LD      HL,0
        ADD     A,A
        LD      C,A
        ADD     HL,BC
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        LD      (AORNAM+1),HL
        SUB     A
        LD      (AENV+1),A
        JP      ADECOM
ACHAN   LD      HL,AQUARK
        DEC     (HL)
        RET     P
ACURQ   LD      (HL),0
APATTR  LD      DE,0
        LD      A,(DE)
        INC     A
        CALL    Z,ENDP
ADECOM  LD      A,(DE)
        INC     DE
        CP      #60
        JR      C,ANOTESL
        CP      #70
        JR      C,ASAMPSL
        CP      #80
        JR      C,AORNSL
        JR      Z,AREST
        CP      #81
        JR      Z,ABACK
        CP      #8F
        JR      C,AENVSL
        SUB     #A1
        LD      (ACURQ+1),A
        LD      (AQUARK),A
        JP      ADECOM
ANOTESL         LD      (ANOTE+1),A
        SUB     A
        LD      (ASMPLAC+1),A
        LD      A,#20
        LD      (AMOD),A
        LD      (APATTR+1),DE
        RET
AREST   LD      A,#FF
        LD      (AMOD),A
ABACK   LD      (APATTR+1),DE
        RET
AZORN   LD      HL,ZERORN
        LD      (AORNAM+1),HL
        SUB     A
        LD      (AENV+1),A
        JP      ADECOM
AENVSL  SUB     #80
        LD      (MODENV),A
        LD      A,(DE)
        LD      (LOWENV),A
        INC     DE
        LD      A,#10
        LD      (AENV+1),A
        LD      (ENSAVE+1),A
        LD      HL,ZERORN
        LD      (AORNAM+1),HL
        JP      ADECOM
BSAMPSL         SUB     #60
BSAMPD  LD      HL,0
        ADD     A,A
        ADD     A,A
        LD      C,A
        ADD     HL,BC
        LD      A,(HL)
        LD      (BREPOS+1),A
        INC     HL
        LD      A,(HL)
        LD      (BSMPLEN+1),A
        INC     HL
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        LD      (BSAMPLE+1),HL
        JP      BDECOM
BORNSL  SUB     #71
        JP      M,BZORN
BORNMD  LD      HL,0
        ADD     A,A
        LD      C,A
        ADD     HL,BC
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        LD      (BORNAM+1),HL
        SUB     A
        LD      (BENV+1),A
        JP      BDECOM
BCHAN   LD      HL,BQUARK
        DEC     (HL)
        RET     P
BCURQ   LD      (HL),0
BPATTR  LD      DE,0
BDECOM  LD      A,(DE)
        INC     DE
        CP      #60
        JR      C,BNOTESL
        CP      #70
        JR      C,BSAMPSL
        CP      #80
        JR      C,BORNSL
        JR      Z,BREST
        CP      #81
        JR      Z,BBACK
        CP      #8F
        JR      C,BENVSL
        SUB     #A1
        LD      (BCURQ+1),A
        LD      (BQUARK),A
        JP      BDECOM
BNOTESL         LD      (BNOTE+1),A
        SUB     A
        LD      (BSMPLAC+1),A
        LD      A,#20
        LD      (BMOD),A
        LD      (BPATTR+1),DE
        RET
BREST   LD      A,#FF
        LD      (BMOD),A
BBACK   LD      (BPATTR+1),DE
        RET
BZORN   LD      HL,ZERORN
        LD      (BORNAM+1),HL
        SUB     A
        LD      (BENV+1),A
        JP      BDECOM
BENVSL  SUB     #80
        LD      (MODENV),A
        LD      A,(DE)
        LD      (LOWENV),A
        INC     DE
        LD      A,#10
        LD      (BENV+1),A
        LD      (ENSAVE+1),A
        LD      HL,ZERORN
        LD      (BORNAM+1),HL
        JP      BDECOM
CSAMPSL         SUB     #60
CSAMPD  LD      HL,0
        ADD     A,A
        ADD     A,A
        LD      C,A
        ADD     HL,BC
        LD      A,(HL)
        LD      (CREPOS+1),A
        INC     HL
        LD      A,(HL)
        LD      (CSMPLEN+1),A
        INC     HL
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        LD      (CSAMPLE+1),HL
        JP      CDECOM
CORNSL  SUB     #71
        JP      M,CZORN
CORNMD  LD      HL,0
        ADD     A,A
        LD      C,A
        ADD     HL,BC
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        LD      (CORNAM+1),HL
        SUB     A
        LD      (CENV+1),A
        JP      CDECOM
CCHAN   LD      HL,CQUARK
        DEC     (HL)
        RET     P
CCURQ   LD      (HL),0
CPATTR  LD      DE,0
CDECOM  LD      A,(DE)
        INC     DE
        CP      #60
        JR      C,CNOTESL
        CP      #70
        JR      C,CSAMPSL
        CP      #80
        JR      C,CORNSL
        JR      Z,CREST
        CP      #81
        JR      Z,CBACK
        CP      #8F
        JR      C,CENVSL
        SUB     #A1
        LD      (CCURQ+1),A
        LD      (CQUARK),A
        JP      CDECOM
CNOTESL         LD      (CNOTE+1),A
        SUB     A
        LD      (CSMPLAC+1),A
        LD      A,#20
        LD      (CMOD),A
        LD      (CPATTR+1),DE
        RET
CREST   LD      A,#FF
        LD      (CMOD),A
CBACK   LD      (CPATTR+1),DE
        RET
CZORN   LD      HL,ZERORN
        LD      (CORNAM+1),HL
        SUB     A
        LD      (CENV+1),A
        JP      CDECOM
CENVSL  SUB     #80
        LD      (MODENV),A
        LD      A,(DE)
        LD      (LOWENV),A
        INC     DE
        LD      A,#10
        LD      (CENV+1),A
        LD      (ENSAVE+1),A
        LD      HL,ZERORN
        LD      (CORNAM+1),HL
        JP      CDECOM
ENDP    LD      (SLPAT+1),SP
PTLS    LD      HL,0
        LD      A,(HL)
        DEC     A
        JP      P,PTLDVE
PTLS2   LD      HL,0
        LD      A,(HL)
        DEC     A
PTLDVE  INC     HL
        LD      (PTLS+1),HL
        ADD     A,A
        LD      C,A
        ADD     A,A
        ADD     A,C
        LD      C,A
CHANDT  LD      HL,0
        ADD     HL,BC
        LD      SP,HL
        POP     DE
        POP     HL
        LD      (BPATTR+1),HL
        POP     HL
        LD      (CPATTR+1),HL
SLPAT   LD      SP,0
        RET
ADVSAMP         LD      HL,AMOD
        LD      A,(HL)
        INC     A
        JP      NZ,ASMPLAC
        LD      (AVOLM),A
        RET
ASMPLAC         LD      A,0
        LD      C,A
        INC     A
        AND     #1F
        LD      (ASMPLAC+1),A
        DEC     (HL)
        JP      NZ,ASAMPLE
AREPOS  LD      A,0
        DEC     A
        JP      P,ALOOPED
        LD      (HL),A
        SUB     A
        LD      (AVOLM),A
        RET
ALOOPED         LD      C,A
        INC     A
        AND     #1F
        LD      (ASMPLAC+1),A
ASMPLEN         LD      (HL),0
ASAMPLE         LD      HL,0
        LD      A,C
        ADD     A,A
        ADD     A,C
        ADD     A,L
        LD      L,A
        ADC     A,H
        SUB     L
        LD      H,A
        LD      A,(HL)
        RRCA
        RRCA
        RRCA
        RRCA
        AND     #0F
        LD      D,A
        LD      A,(HL)
        AND     #0F
AENV    OR      #00
        LD      (AVOLM),A
        INC     HL
        LD      A,(HL)
        AND     A
        LD      B,8
        JP      M,ANNSE
        LD      B,0
        LD      (NOISE),A
ANNSE   BIT     6,A
        JP      Z,APTONE
        INC     B
APTONE  INC     HL
        LD      E,(HL)
        AND     32
        JP      NZ,APLDEV
        LD      HL,0
        SBC     HL,DE
        EX      DE,HL
APLDEV  LD      A,B
        LD      (MIXER),A
AORNAM  LD      HL,0
        LD      B,0
        ADD     HL,BC
ANOTE   LD      A,0
        ADD     A,(HL)
        ADD     A,A
        LD      C,A
        LD      HL,NOTES
        ADD     HL,BC
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        ADD     HL,DE
        LD      (ATON),HL
        RET
BDVSAMP         LD      HL,BMOD
        LD      A,(HL)
        INC     A
        JP      NZ,BSMPLAC
        LD      (BVOLM),A
        RET
BSMPLAC         LD      A,0
        LD      C,A
        INC     A
        AND     #1F
        LD      (BSMPLAC+1),A
        DEC     (HL)
        JP      NZ,BSAMPLE
BREPOS  LD      A,0
        DEC     A
        JP      P,BLOOPED
        LD      (HL),A
        SUB     A
        LD      (BVOLM),A
        RET
BLOOPED         LD      C,A
        INC     A
        AND     #1F
        LD      (BSMPLAC+1),A
BSMPLEN         LD      (HL),0
BSAMPLE         LD      HL,0
        LD      A,C
        ADD     A,A
        ADD     A,C
        ADD     A,L
        LD      L,A
        ADC     A,H
        SUB     L
        LD      H,A
        LD      A,(HL)
        RRCA
        RRCA
        RRCA
        RRCA
        AND     #0F
        LD      D,A
        LD      A,(HL)
        AND     #0F
BENV    OR      #00
        LD      (BVOLM),A
        INC     HL
        LD      A,(HL)
        AND     A
        LD      B,#10
        JP      M,BNNSE
        LD      B,0
        LD      (NOISE),A
BNNSE   BIT     6,A
        JP      Z,BPTONE
        SET     1,B
BPTONE  INC     HL
        LD      E,(HL)
        AND     32
        JP      NZ,BPLDEV
        LD      HL,0
        SBC     HL,DE
        EX      DE,HL
BPLDEV  LD      A,B
        LD      HL,MIXER
        OR      (HL)
        LD      (HL),A
BORNAM  LD      HL,0
        LD      B,0
        ADD     HL,BC
BNOTE   LD      A,0
        ADD     A,(HL)
        ADD     A,A
        LD      C,A
        LD      HL,NOTES
        ADD     HL,BC
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        ADD     HL,DE
        LD      (BTON),HL
        RET
CDVSAMP         LD      HL,CMOD
        LD      A,(HL)
        INC     A
        JP      NZ,CSMPLAC
        LD      (CVOLM),A
        RET
CSMPLAC         LD      A,0
        LD      C,A
        INC     A
        AND     #1F
        LD      (CSMPLAC+1),A
        DEC     (HL)
        JP      NZ,CSAMPLE
CREPOS  LD      A,0
        DEC     A
        JP      P,CLOOPED
        LD      (HL),A
        SUB     A
        LD      (CVOLM),A
        RET
CLOOPED         LD      C,A
        INC     A
        AND     #1F
        LD      (CSMPLAC+1),A
CSMPLEN         LD      (HL),0
CSAMPLE         LD      HL,0
        LD      A,C
        ADD     A,A
        ADD     A,C
        ADD     A,L
        LD      L,A
        ADC     A,H
        SUB     L
        LD      H,A
        LD      A,(HL)
        RRCA
        RRCA
        RRCA
        RRCA
        AND     #0F
        LD      D,A
        LD      A,(HL)
        AND     #0F
CENV    OR      #00
        LD      (CVOLM),A
        INC     HL
        LD      A,(HL)
        AND     A
        LD      B,#20
        JP      M,CNNSE
        LD      B,0
        LD      (NOISE),A
CNNSE   BIT     6,A
        JP      Z,CPTONE
        SET     2,B
CPTONE  INC     HL
        LD      E,(HL)
        AND     32
        JP      NZ,CPLDEV
        LD      HL,0
        SBC     HL,DE
        EX      DE,HL
CPLDEV  LD      A,B
        LD      HL,MIXER
        OR      (HL)
        LD      (HL),A
CORNAM  LD      HL,0
        LD      B,0
        ADD     HL,BC
CNOTE   LD      A,0
        ADD     A,(HL)
        ADD     A,A
        LD      C,A
        LD      HL,NOTES
        ADD     HL,BC
        LD      A,(HL)
        INC     HL
        LD      H,(HL)
        LD      L,A
        ADD     HL,DE
        LD      (CTON),HL
        RET
ZERORN  DEFW    0,0,0,0
        DEFW    0,0,0,0
        DEFW    0,0,0,0
        DEFW    0,0,0,0
ATON    DEFW    0
BTON    DEFW    0
CTON    DEFW    0
NOISE   DEFB    0
MIXER   DEFB    0
AVOLM   DEFB    0
BVOLM   DEFB    0
CVOLM   DEFB    0
LOWENV  DEFB    0
HIHENV  DEFB    0
MODENV  DEFB    0
AQUARK  DEFB    0
BQUARK  DEFB    0
CQUARK  DEFB    0
CURSPD  DEFB    0
AMOD    DEFB    0
BMOD    DEFB    0
CMOD    DEFB    0
NOTES   DEFW    3832
        DEFW    3600,3424,3200,3032
        DEFW    2856,2696,2544,2400
        DEFW    2272,2136,2016,1916
        DEFW    1800,1712,1600,1516
        DEFW    1428,1348,1272,1200
        DEFW    1136,1068,1008,958
        DEFW    900,856,800,758,714
        DEFW    674,636,600,568,534
        DEFW    504,479,450,428,400
        DEFW    379,357,337,318,300
        DEFW    284,267,252,239,225
        DEFW    214,200,189,178,168
        DEFW    159,150,142,133,126
        DEFW    119,112,107,100,94
        DEFW    89,84,79,75,71,66,63
        DEFW    59,56,53,50,47,44,42
        DEFW    39,37,35,33,31,29,28
        DEFW    26,25,23,22,21,19,18
        DEFW    17,16,15
INITROU         LD      (INTSP+1),SP
        LD      SP,HL
        POP     HL
        LD      A,(HL)
        LD      (GLOBSPD+1),A
        INC     HL
        LD      (PTLS+1),HL
        LD      (PTLS2+1),HL
        POP     HL
        LD      (AORNMD+1),HL
        LD      (BORNMD+1),HL
        LD      (CORNMD+1),HL
        POP     HL
        LD      (ASAMPD+1),HL
        LD      (BSAMPD+1),HL
        LD      (CSAMPD+1),HL
        LD      (CHANDT+1),SP
        LD      HL,ZERORN
        LD      (AORNAM+1),HL
        LD      (BORNAM+1),HL
        LD      (CORNAM+1),HL
        LD      HL,ATON
        LD      DE,ATON+1
        LD      BC,17
        LD      (HL),B
        LDIR
        INC     B
        DEC     C
        LD      (HL),B
        INC     HL
        LD      (HL),C
        INC     HL
        LD      (HL),C
        INC     HL
        LD      (HL),C
        LD      (APATTR+1),HL
INTSP   LD      SP,0
        JP      ENSAVE2
