#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "def.h"

#define MAX    512      // 1~2,2~4をそれぞれ512分割、計1024分割
#define MASK9  8372224  // ((1 << 9) - 1) << 14
#define MASK10 8380416  // ((1 << 10) - 1) << 13 
#define MASK14 16383    // 11111111111111


static long long unsigned int make_l[MAX*2];

uint32_t fsqrt(uint32_t a_uint32) {
  
  union data_32bit a, x, result;
  
  a.uint32 = a_uint32;

  if (a.sign == 1) {
    if (a.exp == 0) {
      result.uint32 = NZERO;
    } else {
      result.uint32 = NNAN;
    }
  } else if (a.exp == 0) {
    result.uint32 = ZERO;
  } else if (a.exp == 255 && a.frac != 0) {
    result.uint32 = MY_NAN;
  } else if (a.uint32 == INF) {
    result.uint32 = INF;
  } else {
    
    int index;
    unsigned int exp, y, d, n;
    long long unsigned int l;

    result.sign = 0;
    x.uint32 = a.uint32;

    index = (x.frac & MASK9) >> 14;

    //printf("index = %d (before +1)\n", index); //debug
    
    if ((x.exp & 1) == 0) {      // 2の奇数乗の場合 ※exp-127
      index += (1 << 9);
    } else {
      // 何もしない
    }

    //printf("index = %d (after +1)\n", index); //debug

    
    if (a.exp >= 127) {
      exp = a.exp - 127;
    } else {
      exp = 127 - a.exp;
    }
    
    if ((exp & 1) == 0) {
      x.exp = 127;
    } else {
      x.exp = 128;
    }
    
    if (a.exp >= 127) {
      exp = exp >> 1;
      result.exp = 127 + exp;
    } else {
      exp = (exp + 1) >> 1;
      result.exp = 127 - exp;
    }

    l = make_l[index];
    y = l >> 23;
    d = l & 0x7fffff;
    n = a.frac & MASK14;

    //printf("a.frac = %u\n", a.frac); //debug
    //printf("l = 0x %0llx\n", l); //debug
    //printf("y = %u\n", y); //debug
    //printf("d = %u\n", d); //debug
    //printf("n = %u\n", n); //debug

    result.frac = y + ((d * n) >> 14);

    //printf("result.frac = %23u\n", result.frac); //debug
  }
  return (result.uint32);
}

static long long unsigned int make_l[MAX*2] = {
  0x000000001ffd,
  0x000ffe801ff4,
  0x001ff8801fec,
  0x002fee801fe4,
  0x003fe0801fdc,
  0x004fce801fd4,
  0x005fb8801fcd,
  0x006f9f001fc4,
  0x007f81001fbd,
  0x008f5f801fb5,
  0x009f3a001fae,
  0x00af11001fa5,
  0x00bee3801f9e,
  0x00ceb2801f96,
  0x00de7d801f8e,
  0x00ee44801f87,
  0x00fe08001f7f,
  0x010dc7801f78,
  0x011d83801f70,
  0x012d3b801f68,
  0x013cef801f61,
  0x014ca0001f59,
  0x015c4c801f52,
  0x016bf5801f4a,
  0x017b9a801f43,
  0x018b3c001f3b,
  0x019ad9801f34,
  0x01aa73801f2c,
  0x01ba09801f25,
  0x01c99c001f1e,
  0x01d92b001f17,
  0x01e8b6801f0f,
  0x01f83e001f07,
  0x0207c1801f01,
  0x021742001ef9,
  0x0226be801ef2,
  0x023637801eeb,
  0x0245ad001ee3,
  0x02551e801edd,
  0x02648d001ed5,
  0x0273f7801ece,
  0x02835e801ec7,
  0x0292c2001ec0,
  0x02a222001eb8,
  0x02b17e001eb2,
  0x02c0d7001eab,
  0x02d02c801ea3,
  0x02df7e001e9d,
  0x02eecc801e95,
  0x02fe17001e8f,
  0x030d5e801e87,
  0x031ca2001e81,
  0x032be2801e7a,
  0x033b1f801e73,
  0x034a59001e6c,
  0x03598f001e65,
  0x0368c1801e5e,
  0x0377f0801e58,
  0x03871c801e50,
  0x039644801e4a,
  0x03a569801e43,
  0x03b48b001e3c,
  0x03c3a9001e36,
  0x03d2c4001e2f,
  0x03e1db801e28,
  0x03f0ef801e21,
  0x040000001e1b,
  0x040f0d801e14,
  0x041e17801e0e,
  0x042d1e801e07,
  0x043c22001e00,
  0x044b22001dfa,
  0x045a1f001df3,
  0x046918801dec,
  0x04780e801de6,
  0x048701801de0,
  0x0495f1801dd9,
  0x04a4de001dd3,
  0x04b3c7801dcc,
  0x04c2ad801dc5,
  0x04d190001dbf,
  0x04e06f801db9,
  0x04ef4c001db3,
  0x04fe25801dac,
  0x050cfb801da5,
  0x051bce001d9f,
  0x052a9d801d99,
  0x05396a001d93,
  0x054833801d8c,
  0x0556f9801d86,
  0x0565bc801d80,
  0x05747c801d79,
  0x058339001d74,
  0x0591f3001d6d,
  0x05a0a9801d66,
  0x05af5c801d61,
  0x05be0d001d5a,
  0x05ccba001d55,
  0x05db64801d4e,
  0x05ea0b801d48,
  0x05f8af801d42,
  0x060750801d3b,
  0x0615ee001d36,
  0x062489001d2f,
  0x063320801d2a,
  0x0641b5801d23,
  0x065047001d1e,
  0x065ed6001d17,
  0x066d61801d12,
  0x067bea801d0b,
  0x068a70001d05,
  0x0698f2801d00,
  0x06a772801cf9,
  0x06b5ef001cf4,
  0x06c469001cee,
  0x06d2e0001ce7,
  0x06e153801ce2,
  0x06efc4801cdc,
  0x06fe32801cd6,
  0x070c9d801cd1,
  0x071b06001cca,
  0x07296b001cc5,
  0x0737cd801cbf,
  0x07462d001cb9,
  0x075489801cb3,
  0x0762e3001cad,
  0x077139801ca8,
  0x077f8d801ca2,
  0x078dde801c9d,
  0x079c2d001c96,
  0x07aa78001c91,
  0x07b8c0801c8b,
  0x07c706001c86,
  0x07d549001c80,
  0x07e389001c7a,
  0x07f1c6001c74,
  0x080000001c6f,
  0x080e37801c6a,
  0x081c6c801c63,
  0x082a9e001c5f,
  0x0838cd801c58,
  0x0846f9801c53,
  0x085523001c4e,
  0x08634a001c48,
  0x08716e001c42,
  0x087f8f001c3d,
  0x088dad801c38,
  0x089bc9801c32,
  0x08a9e2801c2c,
  0x08b7f8801c27,
  0x08c60c001c22,
  0x08d41d001c1c,
  0x08e22b001c17,
  0x08f036801c12,
  0x08fe3f801c0c,
  0x090c45801c06,
  0x091a48801c01,
  0x092849001bfc,
  0x093647001bf7,
  0x094442801bf1,
  0x09523b001bec,
  0x096031001be7,
  0x096e24801be1,
  0x097c15001bdc,
  0x098a03001bd7,
  0x0997ee801bd1,
  0x09a5d7001bcc,
  0x09b3bd001bc7,
  0x09c1a0801bc2,
  0x09cf81801bbd,
  0x09dd60001bb7,
  0x09eb3b801bb2,
  0x09f914801bad,
  0x0a06eb001ba8,
  0x0a14bf001ba3,
  0x0a2290801b9d,
  0x0a305f001b98,
  0x0a3e2b001b94,
  0x0a4bf5001b8e,
  0x0a59bc001b89,
  0x0a6780801b84,
  0x0a7542801b7f,
  0x0a8302001b79,
  0x0a90be801b75,
  0x0a9e79001b70,
  0x0aac31001b6a,
  0x0ab9e6001b66,
  0x0ac799001b61,
  0x0ad549801b5b,
  0x0ae2f7001b57,
  0x0af0a2801b51,
  0x0afe4b001b4d,
  0x0b0bf1801b48,
  0x0b1995801b43,
  0x0b2737001b3d,
  0x0b34d5801b39,
  0x0b4272001b34,
  0x0b500c001b2f,
  0x0b5da3801b2a,
  0x0b6b38801b26,
  0x0b78cb801b20,
  0x0b865b801b1b,
  0x0b93e9001b17,
  0x0ba174801b12,
  0x0baefd801b0d,
  0x0bbc84001b08,
  0x0bca08001b03,
  0x0bd789801aff,
  0x0be509001af9,
  0x0bf285801af5,
  0x0c0000001af0,
  0x0c0d78001aec,
  0x0c1aee001ae6,
  0x0c2861001ae2,
  0x0c35d2001add,
  0x0c4340801ad9,
  0x0c50ad001ad3,
  0x0c5e16801acf,
  0x0c6b7e001acb,
  0x0c78e3801ac5,
  0x0c8646001ac1,
  0x0c93a6801abc,
  0x0ca104801ab8,
  0x0cae60801ab3,
  0x0cbbba001aae,
  0x0cc911001aaa,
  0x0cd666001aa5,
  0x0ce3b8801aa0,
  0x0cf108801a9c,
  0x0cfe56801a97,
  0x0d0ba2001a93,
  0x0d18eb801a8e,
  0x0d2632801a8a,
  0x0d3377801a84,
  0x0d40b9801a81,
  0x0d4dfa001a7c,
  0x0d5b38001a77,
  0x0d6873801a73,
  0x0d75ad001a6e,
  0x0d82e4001a6a,
  0x0d9019001a65,
  0x0d9d4b801a61,
  0x0daa7c001a5c,
  0x0db7aa001a58,
  0x0dc4d6001a53,
  0x0dd1ff801a4f,
  0x0ddf27001a4b,
  0x0dec4c801a46,
  0x0df96f801a42,
  0x0e0690801a3d,
  0x0e13af001a39,
  0x0e20cb801a34,
  0x0e2de5801a30,
  0x0e3afd801a2c,
  0x0e4813801a27,
  0x0e5527001a23,
  0x0e6238801a1e,
  0x0e6f47801a1b,
  0x0e7c55001a16,
  0x0e8960001a11,
  0x0e9668801a0d,
  0x0ea36f001a09,
  0x0eb073801a05,
  0x0ebd76001a00,
  0x0eca760019fc,
  0x0ed7740019f8,
  0x0ee4700019f3,
  0x0ef1698019f0,
  0x0efe618019eb,
  0x0f0b570019e6,
  0x0f184a0019e3,
  0x0f253b8019de,
  0x0f322a8019da,
  0x0f3f178019d6,
  0x0f4c028019d2,
  0x0f58eb8019cd,
  0x0f65d20019c9,
  0x0f72b68019c5,
  0x0f7f990019c1,
  0x0f8c798019bd,
  0x0f99580019b8,
  0x0fa6340019b5,
  0x0fb30e8019b0,
  0x0fbfe68019ac,
  0x0fccbc8019a8,
  0x0fd9908019a4,
  0x0fe6628019a0,
  0x0ff33280199b,
  0x100000001998,
  0x100ccc001993,
  0x101995801990,
  0x10265d80198b,
  0x103323001987,
  0x103fe6801983,
  0x104ca8001980,
  0x10596800197b,
  0x106625801977,
  0x1072e1001973,
  0x107f9a80196f,
  0x108c5200196b,
  0x109907801967,
  0x10a5bb001963,
  0x10b26c80195f,
  0x10bf1c00195b,
  0x10cbc9801957,
  0x10d875001953,
  0x10e51e80194f,
  0x10f1c600194b,
  0x10fe6b801947,
  0x110b0f001944,
  0x1117b100193f,
  0x11245080193b,
  0x1130ee001938,
  0x113d8a001933,
  0x114a23801930,
  0x1156bb80192c,
  0x116351801928,
  0x116fe5801924,
  0x117c77801920,
  0x11890780191c,
  0x119595801919,
  0x11a222001914,
  0x11aeac001911,
  0x11bb3480190d,
  0x11c7bb001909,
  0x11d43f801905,
  0x11e0c2001901,
  0x11ed428018fe,
  0x11f9c18018fa,
  0x12063e8018f6,
  0x1212b98018f2,
  0x121f328018ee,
  0x122ba98018eb,
  0x12381f0018e7,
  0x1244928018e3,
  0x1251040018df,
  0x125d738018db,
  0x1269e10018d8,
  0x12764d0018d4,
  0x1282b70018d1,
  0x128f1f8018cc,
  0x129b858018c9,
  0x12a7ea0018c5,
  0x12b44c8018c2,
  0x12c0ad8018bd,
  0x12cd0c0018bb,
  0x12d9698018b6,
  0x12e5c48018b3,
  0x12f21e0018af,
  0x12fe758018ab,
  0x130acb0018a8,
  0x13171f0018a4,
  0x1323710018a0,
  0x132fc100189d,
  0x133c0f801899,
  0x13485c001895,
  0x1354a6801892,
  0x1360ef80188e,
  0x136d3680188b,
  0x13797c001887,
  0x1385bf801883,
  0x139201001880,
  0x139e4100187d,
  0x13aa7f801878,
  0x13b6bb801875,
  0x13c2f6001872,
  0x13cf2f00186e,
  0x13db6600186a,
  0x13e79b001867,
  0x13f3ce801863,
  0x140000001860,
  0x140c3000185c,
  0x14185e001859,
  0x14248a801855,
  0x1430b5001852,
  0x143cde00184e,
  0x14490500184b,
  0x14552a801847,
  0x14614e001843,
  0x146d6f801841,
  0x14799000183c,
  0x1485ae001839,
  0x1491ca801836,
  0x149de5801833,
  0x14a9ff00182e,
  0x14b61600182c,
  0x14c22c001828,
  0x14ce40001824,
  0x14da52001821,
  0x14e66280181e,
  0x14f27180181a,
  0x14fe7e801817,
  0x150a8a001813,
  0x151693801810,
  0x15229b80180d,
  0x152ea2001809,
  0x153aa6801806,
  0x1546a9801802,
  0x1552aa801800,
  0x155eaa8017fb,
  0x156aa80017f9,
  0x1576a48017f5,
  0x15829f0017f1,
  0x158e978017ee,
  0x159a8e8017eb,
  0x15a6840017e8,
  0x15b2780017e4,
  0x15be6a0017e1,
  0x15ca5a8017de,
  0x15d6498017da,
  0x15e2368017d7,
  0x15ee220017d4,
  0x15fa0c0017d0,
  0x1605f40017cd,
  0x1611da8017ca,
  0x161dbf8017c6,
  0x1629a28017c4,
  0x1635848017c0,
  0x1641648017bc,
  0x164d428017ba,
  0x16591f8017b6,
  0x1664fa8017b3,
  0x1670d40017af,
  0x167cab8017ad,
  0x1688820017a9,
  0x1694568017a6,
  0x16a0298017a3,
  0x16abfb00179f,
  0x16b7ca80179c,
  0x16c39880179a,
  0x16cf65801795,
  0x16db30001793,
  0x16e6f9801790,
  0x16f2c180178c,
  0x16fe87801789,
  0x170a4c001786,
  0x17160f001783,
  0x1721d080177f,
  0x172d9000177d,
  0x17394e801779,
  0x17450b001776,
  0x1750c6001773,
  0x175c7f801770,
  0x17683780176c,
  0x1773ed80176a,
  0x177fa2801766,
  0x178b55801764,
  0x179707801760,
  0x17a2b780175d,
  0x17ae6600175a,
  0x17ba13001757,
  0x17c5be801753,
  0x17d168001751,
  0x17dd1080174d,
  0x17e8b700174b,
  0x17f45c801747,
  0x180000001744,
  0x180ba2001742,
  0x18174300173e,
  0x1822e200173b,
  0x182e7f801738,
  0x183a1b801735,
  0x1845b6001732,
  0x18514f00172f,
  0x185ce680172b,
  0x18687c001729,
  0x187410801726,
  0x187fa3801723,
  0x188b35001720,
  0x1896c500171c,
  0x18a25300171a,
  0x18ade0001717,
  0x18b96b801713,
  0x18c4f5001711,
  0x18d07d80170e,
  0x18dc0480170a,
  0x18e789801708,
  0x18f30d801705,
  0x18fe90001702,
  0x190a110016fe,
  0x1915900016fc,
  0x19210e0016f9,
  0x192c8a8016f6,
  0x1938058016f3,
  0x19437f0016f0,
  0x194ef70016ed,
  0x195a6d8016ea,
  0x1965e28016e7,
  0x1971560016e5,
  0x197cc88016e1,
  0x1988390016de,
  0x1993a80016dc,
  0x199f160016d8,
  0x19aa820016d6,
  0x19b5ed0016d3,
  0x19c1568016d0,
  0x19ccbe8016cd,
  0x19d8250016ca,
  0x19e38a0016c7,
  0x19eeed8016c4,
  0x19fa4f8016c2,
  0x1a05b08016be,
  0x1a110f8016bc,
  0x1a1c6d8016b9,
  0x1a27ca0016b6,
  0x1a33250016b3,
  0x1a3e7e8016b0,
  0x1a49d68016ad,
  0x1a552d0016ab,
  0x1a60828016a7,
  0x1a6bd60016a5,
  0x1a77288016a2,
  0x1a827a002d3b,
  0x1a9917802d31,
  0x1aafb0002d25,
  0x1ac642802d1a,
  0x1adccf802d0e,
  0x1af356802d04,
  0x1b09d8802cf8,
  0x1b2054802cee,
  0x1b36cb802ce2,
  0x1b4d3c802cd7,
  0x1b63a8002ccc,
  0x1b7a0e002cc2,
  0x1b906f002cb6,
  0x1ba6ca002cab,
  0x1bbd1f802ca1,
  0x1bd370002c96,
  0x1be9bb002c8b,
  0x1c0000802c80,
  0x1c1640802c75,
  0x1c2c7b002c6b,
  0x1c42b0802c60,
  0x1c58e0802c55,
  0x1c6f0b002c4b,
  0x1c8530802c40,
  0x1c9b50802c36,
  0x1cb16b802c2b,
  0x1cc781002c21,
  0x1cdd91802c16,
  0x1cf39c802c0b,
  0x1d09a2002c02,
  0x1d1fa3002bf7,
  0x1d359e802bec,
  0x1d4b94802be2,
  0x1d6185802bd8,
  0x1d7771802bce,
  0x1d8d58802bc3,
  0x1da33a002bb9,
  0x1db916802baf,
  0x1dceee002ba5,
  0x1de4c0802b9b,
  0x1dfa8e002b90,
  0x1e1056002b87,
  0x1e2619802b7c,
  0x1e3bd7802b72,
  0x1e5190802b69,
  0x1e6745002b5e,
  0x1e7cf4002b55,
  0x1e929e802b4a,
  0x1ea843802b41,
  0x1ebde4002b37,
  0x1ed37f802b2d,
  0x1ee916002b23,
  0x1efea7802b19,
  0x1f1434002b10,
  0x1f29bc002b06,
  0x1f3f3f002afc,
  0x1f54bd002af2,
  0x1f6a36002ae9,
  0x1f7faa802adf,
  0x1f951a002ad6,
  0x1faa85002acc,
  0x1fbfeb002ac2,
  0x1fd54c002ab9,
  0x1feaa8802ab0,
  0x200000802aa6,
  0x201553802a9c,
  0x202aa1802a93,
  0x203feb002a8a,
  0x205530002a80,
  0x206a70002a77,
  0x207fab802a6d,
  0x2094e2002a65,
  0x20aa14802a5b,
  0x20bf42002a51,
  0x20d46a802a49,
  0x20e98f002a3f,
  0x20feae802a36,
  0x2113c9802a2d,
  0x2128e0002a24,
  0x213df2002a1a,
  0x2152ff002a12,
  0x216808002a08,
  0x217d0c0029ff,
  0x21920b8029f7,
  0x21a7070029ed,
  0x21bbfd8029e5,
  0x21d0f00029db,
  0x21e5dd8029d2,
  0x21fac68029ca,
  0x220fab8029c0,
  0x22248b8029b8,
  0x2239678029af,
  0x224e3f0029a6,
  0x22631200299e,
  0x2277e1002994,
  0x228cab00298c,
  0x22a171002983,
  0x22b63280297a,
  0x22caef802971,
  0x22dfa8002969,
  0x22f45c802961,
  0x23090d002957,
  0x231db880294f,
  0x233260002946,
  0x23470300293e,
  0x235ba2002936,
  0x23703d00292c,
  0x2384d3002924,
  0x23996500291c,
  0x23adf3002913,
  0x23c27c80290b,
  0x23d702002903,
  0x23eb838028fa,
  0x2400008028f1,
  0x2414790028e9,
  0x2428ed8028e1,
  0x243d5e0028d9,
  0x2451ca8028d0,
  0x2466328028c8,
  0x247a968028bf,
  0x248ef60028b8,
  0x24a3520028af,
  0x24b7a98028a7,
  0x24cbfd00289f,
  0x24e04c802896,
  0x24f49780288f,
  0x2508df002886,
  0x251d2200287e,
  0x253161002876,
  0x25459c00286e,
  0x2559d3002866,
  0x256e0600285e,
  0x258235002856,
  0x25966000284e,
  0x25aa87002846,
  0x25beaa00283e,
  0x25d2c9002836,
  0x25e6e400282e,
  0x25fafb002826,
  0x260f0e00281f,
  0x26231d802816,
  0x26372880280f,
  0x264b30002806,
  0x265f330027ff,
  0x2673328027f7,
  0x26872e0027f0,
  0x269b260027e7,
  0x26af198027e0,
  0x26c3098027d8,
  0x26d6f58027d1,
  0x26eade0027c8,
  0x26fec20027c1,
  0x2712a28027ba,
  0x27267f8027b1,
  0x273a580027aa,
  0x274e2d0027a3,
  0x2761fe80279b,
  0x2775cc002793,
  0x27899580278c,
  0x279d5b802784,
  0x27b11d80277c,
  0x27c4db802776,
  0x27d89680276d,
  0x27ec4d002766,
  0x28000000275f,
  0x2813af802757,
  0x28275b002750,
  0x283b03002749,
  0x284ea7802741,
  0x28624800273a,
  0x2875e5002732,
  0x28897e00272b,
  0x289d13802724,
  0x28b0a580271c,
  0x28c433802715,
  0x28d7be00270e,
  0x28eb45002706,
  0x28fec8002700,
  0x2912480026f8,
  0x2925c40026f0,
  0x29393c0026ea,
  0x294cb10026e2,
  0x2960220026dc,
  0x2973900026d4,
  0x2986fa0026cd,
  0x299a608026c6,
  0x29adc38026be,
  0x29c1228026b8,
  0x29d47e8026b1,
  0x29e7d70026a9,
  0x29fb2b8026a3,
  0x2a0e7d00269b,
  0x2a21ca802694,
  0x2a351480268e,
  0x2a485b802686,
  0x2a5b9e802680,
  0x2a6ede802678,
  0x2a821a802672,
  0x2a955380266a,
  0x2aa888802664,
  0x2abbba80265d,
  0x2acee9002656,
  0x2ae21400264f,
  0x2af53b802648,
  0x2b085f802642,
  0x2b1b8080263a,
  0x2b2e9d802634,
  0x2b41b780262d,
  0x2b54ce002626,
  0x2b67e100261f,
  0x2b7af0802619,
  0x2b8dfd002612,
  0x2ba10600260b,
  0x2bb40b802604,
  0x2bc70d8025fe,
  0x2bda0c8025f7,
  0x2bed080025f0,
  0x2c00000025ea,
  0x2c12f50025e3,
  0x2c25e68025dd,
  0x2c38d50025d5,
  0x2c4bbf8025d0,
  0x2c5ea78025c8,
  0x2c718b8025c2,
  0x2c846c8025bc,
  0x2c974a8025b5,
  0x2caa250025ae,
  0x2cbcfc0025a8,
  0x2ccfd00025a1,
  0x2ce2a080259b,
  0x2cf56e002595,
  0x2d083880258d,
  0x2d1aff002588,
  0x2d2dc3002581,
  0x2d408380257a,
  0x2d5340802575,
  0x2d65fb00256d,
  0x2d78b1802568,
  0x2d8b65802561,
  0x2d9e1600255a,
  0x2db0c3002554,
  0x2dc36d00254e,
  0x2dd614002548,
  0x2de8b8002541,
  0x2dfb5880253b,
  0x2e0df6002535,
  0x2e209080252e,
  0x2e3327802528,
  0x2e45bb802522,
  0x2e584c80251c,
  0x2e6ada802515,
  0x2e7d6500250f,
  0x2e8fec802509,
  0x2ea271002503,
  0x2eb4f28024fc,
  0x2ec7708024f7,
  0x2ed9ec0024f0,
  0x2eec640024ea,
  0x2efed90024e4,
  0x2f114b0024de,
  0x2f23ba0024d7,
  0x2f36258024d2,
  0x2f488e8024cc,
  0x2f5af48024c5,
  0x2f6d570024bf,
  0x2f7fb68024ba,
  0x2f92138024b3,
  0x2fa46d0024ad,
  0x2fb6c38024a8,
  0x2fc9178024a1,
  0x2fdb6800249b,
  0x2fedb5802495,
  0x300000002490,
  0x301248002489,
  0x30248c802483,
  0x3036ce00247e,
  0x30490d002477,
  0x305b48802472,
  0x306d8180246c,
  0x307fb7802466,
  0x3091ea80245f,
  0x30a41a00245b,
  0x30b647802454,
  0x30c87180244e,
  0x30da98802449,
  0x30ecbd002442,
  0x30fede00243d,
  0x3110fc802437,
  0x312318002432,
  0x31353100242b,
  0x314746802426,
  0x315959802420,
  0x316b6980241a,
  0x317d76802415,
  0x318f8100240e,
  0x31a188002409,
  0x31b38c802404,
  0x31c58e8023fd,
  0x31d78d0023f8,
  0x31e9890023f3,
  0x31fb828023ec,
  0x320d788023e7,
  0x321f6c0023e2,
  0x32315d0023db,
  0x32434a8023d6,
  0x3255358023d1,
  0x32671e0023cb,
  0x3279038023c5,
  0x328ae60023c0,
  0x329cc60023ba,
  0x32aea30023b4,
  0x32c07d0023af,
  0x32d2548023aa,
  0x32e4298023a4,
  0x32f5fb80239e,
  0x3307ca802399,
  0x331997002393,
  0x332b6080238e,
  0x333d27802389,
  0x334eec002382,
  0x3360ad00237e,
  0x33726c002378,
  0x338428002373,
  0x3395e180236d,
  0x33a798002367,
  0x33b94b802363,
  0x33cafd00235d,
  0x33dcab802357,
  0x33ee57002352,
  0x34000000234d,
  0x3411a6802347,
  0x34234a002343,
  0x3434eb80233c,
  0x344689802338,
  0x345825802332,
  0x3469be80232c,
  0x347b54802328,
  0x348ce8802322,
  0x349e7980231d,
  0x34b008002317,
  0x34c193802313,
  0x34d31c80230e,
  0x34e4a3802307,
  0x34f627002303,
  0x3507a88022fd,
  0x3519270022f8,
  0x352aa30022f3,
  0x353c1c8022ee,
  0x354d938022e8,
  0x355f078022e3,
  0x3570790022de,
  0x3581e80022d9,
  0x3593548022d4,
  0x35a4be8022cf,
  0x35b6260022c9,
  0x35c78a8022c4,
  0x35d8ec8022c0,
  0x35ea4c8022ba,
  0x35fba98022b5,
  0x360d040022af,
  0x361e5b8022ab,
  0x362fb10022a6,
  0x3641040022a1,
  0x36525480229b,
  0x3663a2002297,
  0x3674ed802291,
  0x36863600228c,
  0x36977c002288,
  0x36a8c0002282,
  0x36ba0100227e,
  0x36cb40002278,
  0x36dc7c002273,
  0x36edb580226f,
  0x36feed002269,
  0x371021802264,
  0x372153802260,
  0x37328380225a,
  0x3743b0802256,
  0x3754db802251,
  0x37660400224b,
  0x377729802247,
  0x37884d002242,
  0x37996e00223d,
  0x37aa8c802238,
  0x37bba8802233,
  0x37ccc200222e,
  0x37ddd900222a,
  0x37eeee002224,
  0x380000002220,
  0x38111000221b,
  0x38221d802216,
  0x383328802211,
  0x38443100220c,
  0x385537002208,
  0x38663b002203,
  0x38773c8021fe,
  0x38883b8021f9,
  0x3899380021f4,
  0x38aa320021f0,
  0x38bb2a0021eb,
  0x38cc1f8021e6,
  0x38dd128021e1,
  0x38ee030021dd,
  0x38fef18021d8,
  0x390fdd8021d3,
  0x3920c70021ce,
  0x3931ae0021ca,
  0x3942930021c5,
  0x3953758021c0,
  0x3964558021bc,
  0x3975338021b6,
  0x39860e8021b3,
  0x3996e80021ad,
  0x39a7be8021a9,
  0x39b8930021a4,
  0x39c9650021a0,
  0x39da3500219b,
  0x39eb02802196,
  0x39fbcd802192,
  0x3a0c9680218d,
  0x3a1d5d002189,
  0x3a2e21802184,
  0x3a3ee380217f,
  0x3a4fa300217b,
  0x3a6060802176,
  0x3a711b802171,
  0x3a81d400216e,
  0x3a928b002168,
  0x3aa33f002164,
  0x3ab3f100215f,
  0x3ac4a080215b,
  0x3ad54e002156,
  0x3ae5f9002152,
  0x3af6a200214e,
  0x3b0749002148,
  0x3b17ed002145,
  0x3b288f80213f,
  0x3b392f00213c,
  0x3b49cd002137,
  0x3b5a68802132,
  0x3b6b0180212e,
  0x3b7b98802129,
  0x3b8c2d002125,
  0x3b9cbf802121,
  0x3bad5000211c,
  0x3bbdde002118,
  0x3bce6a002113,
  0x3bdef380210f,
  0x3bef7b00210a,
  0x3c0000002106,
  0x3c1083002102,
  0x3c21040020fd,
  0x3c31828020f9,
  0x3c41ff0020f5,
  0x3c52798020f0,
  0x3c62f18020ec,
  0x3c73678020e7,
  0x3c83db0020e3,
  0x3c944c8020df,
  0x3ca4bc0020da,
  0x3cb5290020d7,
  0x3cc5948020d1,
  0x3cd5fd0020ce,
  0x3ce6640020c9,
  0x3cf6c88020c5,
  0x3d072b0020c0,
  0x3d178b0020bd,
  0x3d27e98020b8,
  0x3d38458020b4,
  0x3d489f8020af,
  0x3d58f70020ab,
  0x3d694c8020a7,
  0x3d79a00020a3,
  0x3d89f180209e,
  0x3d9a4080209b,
  0x3daa8e002096,
  0x3dbad9002091,
  0x3dcb2180208e,
  0x3ddb68802089,
  0x3debad002086,
  0x3dfbf0002081,
  0x3e0c3080207c,
  0x3e1c6e802079,
  0x3e2cab002074,
  0x3e3ce5002071,
  0x3e4d1d80206c,
  0x3e5d53802068,
  0x3e6d87802064,
  0x3e7db980205f,
  0x3e8de900205c,
  0x3e9e17002057,
  0x3eae42802053,
  0x3ebe6c00204f,
  0x3ece9380204b,
  0x3edeb9002047,
  0x3eeedc802043,
  0x3efefe00203f,
  0x3f0f1d80203b,
  0x3f1f3b002036,
  0x3f2f56002033,
  0x3f3f6f80202e,
  0x3f4f8680202a,
  0x3f5f9b802027,
  0x3f6faf002022,
  0x3f7fc000201e,
  0x3f8fcf00201a,
  0x3f9fdc002016,
  0x3fafe7002012,
  0x3fbff000200e,
  0x3fcff700200a,
  0x3fdffc002006,
  0x3fefff002002,
};
