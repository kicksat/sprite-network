# Generates and stores Gold Codes in a .h file

mseq1 = [
    1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0,
    1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1,
    0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0,
    1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1,
    1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0,
    0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1,
    1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1,
    0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1,
    0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1,
    0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0,
    0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1
]

mseq2 = [
    1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0,
    1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0,
    1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1,
    0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0,
    1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1,
    1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1,
    0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0,
    0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1,
    0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1,
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0
]


def generate_prns(prn_id0, prn_id1):
    M_SEQUENCE_LENGTH = 511
    SPRITE_PRN_LENGTH = 512
    m_prn0 = []
    m_prn1 = []

    if prn_id0 == -2:
        #Deep copy M-sequence
        for k in range(0,M_SEQUENCE_LENGTH):
            m_prn0[k] = mseq1[k];

    elif(prn_id0 == -1):
        #Deep copy M-sequence
        for k in range(0,M_SEQUENCE_LENGTH):
            m_prn0[k] = mseq2[k];

    else:   #if(prn_id >= 0 && prn_id < M_SEQUENCE_LENGTH)
        #Generate Gold Codes by xor'ing 2 M-sequences in different phases
        for k in range(0,M_SEQUENCE_LENGTH-prn_id0):
            m_prn0.append(mseq1[k] ^ mseq2[k+prn_id0])

        for k in range(M_SEQUENCE_LENGTH-prn_id0,M_SEQUENCE_LENGTH):
            m_prn0.append(mseq1[k] ^ mseq2[k-M_SEQUENCE_LENGTH+prn_id0])

    m_prn0.append(0) #To pad out the last byte, add a zero to the end


    if prn_id1 == -2:
        #Deep copy M-sequence
        for k in range(0,M_SEQUENCE_LENGTH):
            m_prn1[k] = mseq1[k];
    elif(prn_id1 == -1):
        #Deep copy M-sequence
        for k in range(0,M_SEQUENCE_LENGTH):
            m_prn1[k] = mseq2[k];

    else:   #if(prn_id >= 0 && prn_id < M_SEQUENCE_LENGTH)
        #Generate Gold Codes by xor'ing 2 M-sequences in different phases
        for k in range(0,M_SEQUENCE_LENGTH-prn_id1):
            m_prn1.append(mseq1[k] ^ mseq2[k+prn_id1])

        for k in range(M_SEQUENCE_LENGTH-prn_id1,M_SEQUENCE_LENGTH):
            m_prn1.append(mseq1[k] ^ mseq2[k-M_SEQUENCE_LENGTH+prn_id1])

        m_prn1.append(0); #To pad out the last byte, add a zero to the end

    return (m_prn0,m_prn1)


def byteFormat(prn0):
    prnByte0 = []
    for k in range(0,511,8):
        prnByte0.append("0b" + str (prn0[k]) + str (prn0[k+1]) 
            + str (prn0[k+2])  + str (prn0[k+3]) + str (prn0[k+4])
             + str (prn0[k+5]) + str (prn0[k+6]) + str (prn0[k+7]))
    return(prnByte0)


def __main__():
    # Overwrite existing file
    with open('GoldCodes.h', 'w') as f:
        f.write("#ifndef GOLD_CODES_H\n#define GOLD_CODES_H\n\n")

    for n in range(2,25,2):
        (prn0, prn1) = generate_prns(n,n+1)
        prnByte0 = byteFormat(prn0)
        prnByte1 = byteFormat(prn1)

        with open('GoldCodes.h', 'a') as f:
            #f.write('// prnID {0} \nunsigned char prn0[64]= {{'.format(n))
            f.write('#define prn{0} {{'.format(n))
            for b in range(0,63):
                f.write('{0}, '.format(prnByte0[b]))
            f.write(prnByte0[63])
            f.write('}\n\n')
            f.write('#define prn{0} {{'.format(n+1))
            for b in range(0,63):
                f.write('{0}, '.format(prnByte1[b]))
            f.write('{}'.format(prnByte1[63]))
            f.write('}\n\n\n')
    with open('GoldCodes.h', 'a') as f:
        f.write("#endif")


__main__()