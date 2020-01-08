// defines the data structure used in protect mode

#ifndef _ORANGES_PROTECT_H_
#define _ORANGES_PROTECT_H_

// descriptor for data/system seg, 8 bytes
// refer to pm.inc
typedef struct s_descriptor {
    u16    limit_low;
    u16    base_low;
    u8     base_mid;
    u8     attr1;
    u8     limit_high_attr2;
    u8     base_high
}DESCRIPTOR;

#endif /* _ORANGES_PROTECT_H_ */