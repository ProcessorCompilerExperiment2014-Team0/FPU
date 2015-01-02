#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "def.h"

#define MAX    512      // 1~2,2~4をそれぞれ512分割、計1024分割
#define MASK9  8372224  // ((1 << 9) - 1) << 14
#define MASK10 8380416  // ((1 << 10) - 1) << 13 
#define MASK13 16382    // 11111111111110


long long unsigned int make_l(int index);

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

    printf("index = %d (before +1)\n", index); //debug
    
    if ((x.exp & 1) == 0) {      // 2の奇数乗の場合 ※exp-127
      index += (1 << 9);
    } else {
      // 何もしない
    }

    printf("index = %d (after +1)\n", index); //debug

    
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

    l = make_l(index);
    y = (l & (0x7fffff << 13)) >> 13;
    d = l & 0x1fff;
    n = a.frac & MASK13;

    printf("a.frac = %u\n", a.frac); //debug
    printf("l = 0x %0llx\n", l); //debug
    printf("y = %u\n", y); //debug
    printf("d = %u\n", d); //debug
    printf("n = %u\n", n); //debug

    result.frac = y + ((d * n) >> 12);

    printf("result.frac = %23u\n", result.frac); //debug
  }
  return (result.uint32);
}

long long unsigned int make_l(int index) {
  long long unsigned int l;
  switch (index) {
  case    0:
    l = 0x000001ffd; break;
  case    1:
    l = 0x003ffbff4; break;
  case    2:
    l = 0x007fe3fec; break;
  case    3:
    l = 0x00bfbbfe4; break;
  case    4:
    l = 0x00ff83fdc; break;
  case    5:
    l = 0x013f3bfd4; break;
  case    6:
    l = 0x017ee3fcd; break;
  case    7:
    l = 0x01be7dfc4; break;
  case    8:
    l = 0x01fe05fbd; break;
  case    9:
    l = 0x023d7ffb5; break;
  case   10:
    l = 0x027ce9fae; break;
  case   11:
    l = 0x02bc45fa5; break;
  case   12:
    l = 0x02fb8ff9e; break;
  case   13:
    l = 0x033acbf96; break;
  case   14:
    l = 0x0379f7f8e; break;
  case   15:
    l = 0x03b913f87; break;
  case   16:
    l = 0x03f821f7f; break;
  case   17:
    l = 0x04371ff78; break;
  case   18:
    l = 0x04760ff70; break;
  case   19:
    l = 0x04b4eff68; break;
  case   20:
    l = 0x04f3bff61; break;
  case   21:
    l = 0x053281f59; break;
  case   22:
    l = 0x057133f52; break;
  case   23:
    l = 0x05afd7f4a; break;
  case   24:
    l = 0x05ee6bf43; break;
  case   25:
    l = 0x062cf1f3b; break;
  case   26:
    l = 0x066b67f34; break;
  case   27:
    l = 0x06a9cff2c; break;
  case   28:
    l = 0x06e827f25; break;
  case   29:
    l = 0x072671f1e; break;
  case   30:
    l = 0x0764adf17; break;
  case   31:
    l = 0x07a2dbf0f; break;
  case   32:
    l = 0x07e0f9f07; break;
  case   33:
    l = 0x081f07f01; break;
  case   34:
    l = 0x085d09ef9; break;
  case   35:
    l = 0x089afbef2; break;
  case   36:
    l = 0x08d8dfeeb; break;
  case   37:
    l = 0x0916b5ee3; break;
  case   38:
    l = 0x09547bedd; break;
  case   39:
    l = 0x099235ed5; break;
  case   40:
    l = 0x09cfdfece; break;
  case   41:
    l = 0x0a0d7bec7; break;
  case   42:
    l = 0x0a4b09ec0; break;
  case   43:
    l = 0x0a8889eb8; break;
  case   44:
    l = 0x0ac5f9eb2; break;
  case   45:
    l = 0x0b035deab; break;
  case   46:
    l = 0x0b40b3ea3; break;
  case   47:
    l = 0x0b7df9e9d; break;
  case   48:
    l = 0x0bbb33e95; break;
  case   49:
    l = 0x0bf85de8f; break;
  case   50:
    l = 0x0c357be87; break;
  case   51:
    l = 0x0c7289e81; break;
  case   52:
    l = 0x0caf8be7a; break;
  case   53:
    l = 0x0cec7fe73; break;
  case   54:
    l = 0x0d2965e6c; break;
  case   55:
    l = 0x0d663de65; break;
  case   56:
    l = 0x0da307e5e; break;
  case   57:
    l = 0x0ddfc3e58; break;
  case   58:
    l = 0x0e1c73e50; break;
  case   59:
    l = 0x0e5913e4a; break;
  case   60:
    l = 0x0e95a7e43; break;
  case   61:
    l = 0x0ed22de3c; break;
  case   62:
    l = 0x0f0ea5e36; break;
  case   63:
    l = 0x0f4b11e2f; break;
  case   64:
    l = 0x0f876fe28; break;
  case   65:
    l = 0x0fc3bfe21; break;
  case   66:
    l = 0x100001e1b; break;
  case   67:
    l = 0x103c37e14; break;
  case   68:
    l = 0x10785fe0e; break;
  case   69:
    l = 0x10b47be07; break;
  case   70:
    l = 0x10f089e00; break;
  case   71:
    l = 0x112c89dfa; break;
  case   72:
    l = 0x11687ddf3; break;
  case   73:
    l = 0x11a463dec; break;
  case   74:
    l = 0x11e03bde6; break;
  case   75:
    l = 0x121c07de0; break;
  case   76:
    l = 0x1257c7dd9; break;
  case   77:
    l = 0x129379dd3; break;
  case   78:
    l = 0x12cf1fdcc; break;
  case   79:
    l = 0x130ab7dc5; break;
  case   80:
    l = 0x134641dbf; break;
  case   81:
    l = 0x1381bfdb9; break;
  case   82:
    l = 0x13bd31db3; break;
  case   83:
    l = 0x13f897dac; break;
  case   84:
    l = 0x1433efda5; break;
  case   85:
    l = 0x146f39d9f; break;
  case   86:
    l = 0x14aa77d99; break;
  case   87:
    l = 0x14e5a9d93; break;
  case   88:
    l = 0x1520cfd8c; break;
  case   89:
    l = 0x155be7d86; break;
  case   90:
    l = 0x1596f3d80; break;
  case   91:
    l = 0x15d1f3d79; break;
  case   92:
    l = 0x160ce5d74; break;
  case   93:
    l = 0x1647cdd6d; break;
  case   94:
    l = 0x1682a7d66; break;
  case   95:
    l = 0x16bd73d61; break;
  case   96:
    l = 0x16f835d5a; break;
  case   97:
    l = 0x1732e9d55; break;
  case   98:
    l = 0x176d93d4e; break;
  case   99:
    l = 0x17a82fd48; break;
  case  100:
    l = 0x17e2bfd42; break;
  case  101:
    l = 0x181d43d3b; break;
  case  102:
    l = 0x1857b9d36; break;
  case  103:
    l = 0x189225d2f; break;
  case  104:
    l = 0x18cc83d2a; break;
  case  105:
    l = 0x1906d7d23; break;
  case  106:
    l = 0x19411dd1e; break;
  case  107:
    l = 0x197b59d17; break;
  case  108:
    l = 0x19b587d12; break;
  case  109:
    l = 0x19efabd0b; break;
  case  110:
    l = 0x1a29c1d05; break;
  case  111:
    l = 0x1a63cbd00; break;
  case  112:
    l = 0x1a9dcbcf9; break;
  case  113:
    l = 0x1ad7bdcf4; break;
  case  114:
    l = 0x1b11a5cee; break;
  case  115:
    l = 0x1b4b81ce7; break;
  case  116:
    l = 0x1b854fce2; break;
  case  117:
    l = 0x1bbf13cdc; break;
  case  118:
    l = 0x1bf8cbcd6; break;
  case  119:
    l = 0x1c3277cd1; break;
  case  120:
    l = 0x1c6c19cca; break;
  case  121:
    l = 0x1ca5adcc5; break;
  case  122:
    l = 0x1cdf37cbf; break;
  case  123:
    l = 0x1d18b5cb9; break;
  case  124:
    l = 0x1d5227cb3; break;
  case  125:
    l = 0x1d8b8dcad; break;
  case  126:
    l = 0x1dc4e7ca8; break;
  case  127:
    l = 0x1dfe37ca2; break;
  case  128:
    l = 0x1e377bc9d; break;
  case  129:
    l = 0x1e70b5c96; break;
  case  130:
    l = 0x1ea9e1c91; break;
  case  131:
    l = 0x1ee303c8b; break;
  case  132:
    l = 0x1f1c19c86; break;
  case  133:
    l = 0x1f5525c80; break;
  case  134:
    l = 0x1f8e25c7a; break;
  case  135:
    l = 0x1fc719c74; break;
  case  136:
    l = 0x200001c6f; break;
  case  137:
    l = 0x2038dfc6a; break;
  case  138:
    l = 0x2071b3c63; break;
  case  139:
    l = 0x20aa79c5f; break;
  case  140:
    l = 0x20e337c58; break;
  case  141:
    l = 0x211be7c53; break;
  case  142:
    l = 0x21548dc4e; break;
  case  143:
    l = 0x218d29c48; break;
  case  144:
    l = 0x21c5b9c42; break;
  case  145:
    l = 0x21fe3dc3d; break;
  case  146:
    l = 0x2236b7c38; break;
  case  147:
    l = 0x226f27c32; break;
  case  148:
    l = 0x22a78bc2c; break;
  case  149:
    l = 0x22dfe3c27; break;
  case  150:
    l = 0x231831c22; break;
  case  151:
    l = 0x235075c1c; break;
  case  152:
    l = 0x2388adc17; break;
  case  153:
    l = 0x23c0dbc12; break;
  case  154:
    l = 0x23f8ffc0c; break;
  case  155:
    l = 0x243117c06; break;
  case  156:
    l = 0x246923c01; break;
  case  157:
    l = 0x24a125bfc; break;
  case  158:
    l = 0x24d91dbf7; break;
  case  159:
    l = 0x25110bbf1; break;
  case  160:
    l = 0x2548edbec; break;
  case  161:
    l = 0x2580c5be7; break;
  case  162:
    l = 0x25b893be1; break;
  case  163:
    l = 0x25f055bdc; break;
  case  164:
    l = 0x26280dbd7; break;
  case  165:
    l = 0x265fbbbd1; break;
  case  166:
    l = 0x26975dbcc; break;
  case  167:
    l = 0x26cef5bc7; break;
  case  168:
    l = 0x270683bc2; break;
  case  169:
    l = 0x273e07bbd; break;
  case  170:
    l = 0x277581bb7; break;
  case  171:
    l = 0x27acefbb2; break;
  case  172:
    l = 0x27e453bad; break;
  case  173:
    l = 0x281badba8; break;
  case  174:
    l = 0x2852fdba3; break;
  case  175:
    l = 0x288a43b9d; break;
  case  176:
    l = 0x28c17db98; break;
  case  177:
    l = 0x28f8adb94; break;
  case  178:
    l = 0x292fd5b8e; break;
  case  179:
    l = 0x2966f1b89; break;
  case  180:
    l = 0x299e03b84; break;
  case  181:
    l = 0x29d50bb7f; break;
  case  182:
    l = 0x2a0c09b79; break;
  case  183:
    l = 0x2a42fbb75; break;
  case  184:
    l = 0x2a79e5b70; break;
  case  185:
    l = 0x2ab0c5b6a; break;
  case  186:
    l = 0x2ae799b66; break;
  case  187:
    l = 0x2b1e65b61; break;
  case  188:
    l = 0x2b5527b5b; break;
  case  189:
    l = 0x2b8bddb57; break;
  case  190:
    l = 0x2bc28bb51; break;
  case  191:
    l = 0x2bf92db4d; break;
  case  192:
    l = 0x2c2fc7b48; break;
  case  193:
    l = 0x2c6657b43; break;
  case  194:
    l = 0x2c9cddb3d; break;
  case  195:
    l = 0x2cd357b39; break;
  case  196:
    l = 0x2d09c9b34; break;
  case  197:
    l = 0x2d4031b2f; break;
  case  198:
    l = 0x2d768fb2a; break;
  case  199:
    l = 0x2dace3b26; break;
  case  200:
    l = 0x2de32fb20; break;
  case  201:
    l = 0x2e196fb1b; break;
  case  202:
    l = 0x2e4fa5b17; break;
  case  203:
    l = 0x2e85d3b12; break;
  case  204:
    l = 0x2ebbf7b0d; break;
  case  205:
    l = 0x2ef211b08; break;
  case  206:
    l = 0x2f2821b03; break;
  case  207:
    l = 0x2f5e27aff; break;
  case  208:
    l = 0x2f9425af9; break;
  case  209:
    l = 0x2fca17af5; break;
  case  210:
    l = 0x300001af0; break;
  case  211:
    l = 0x3035e1aec; break;
  case  212:
    l = 0x306bb9ae6; break;
  case  213:
    l = 0x30a185ae2; break;
  case  214:
    l = 0x30d749add; break;
  case  215:
    l = 0x310d03ad9; break;
  case  216:
    l = 0x3142b5ad3; break;
  case  217:
    l = 0x31785bacf; break;
  case  218:
    l = 0x31adf9acb; break;
  case  219:
    l = 0x31e38fac5; break;
  case  220:
    l = 0x321919ac1; break;
  case  221:
    l = 0x324e9babc; break;
  case  222:
    l = 0x328413ab8; break;
  case  223:
    l = 0x32b983ab3; break;
  case  224:
    l = 0x32eee9aae; break;
  case  225:
    l = 0x332445aaa; break;
  case  226:
    l = 0x335999aa5; break;
  case  227:
    l = 0x338ee3aa0; break;
  case  228:
    l = 0x33c423a9c; break;
  case  229:
    l = 0x33f95ba97; break;
  case  230:
    l = 0x342e89a93; break;
  case  231:
    l = 0x3463afa8e; break;
  case  232:
    l = 0x3498cba8a; break;
  case  233:
    l = 0x34cddfa84; break;
  case  234:
    l = 0x3502e7a81; break;
  case  235:
    l = 0x3537e9a7c; break;
  case  236:
    l = 0x356ce1a77; break;
  case  237:
    l = 0x35a1cfa73; break;
  case  238:
    l = 0x35d6b5a6e; break;
  case  239:
    l = 0x360b91a6a; break;
  case  240:
    l = 0x364065a65; break;
  case  241:
    l = 0x36752fa61; break;
  case  242:
    l = 0x36a9f1a5c; break;
  case  243:
    l = 0x36dea9a58; break;
  case  244:
    l = 0x371359a53; break;
  case  245:
    l = 0x3747ffa4f; break;
  case  246:
    l = 0x377c9da4b; break;
  case  247:
    l = 0x37b133a46; break;
  case  248:
    l = 0x37e5bfa42; break;
  case  249:
    l = 0x381a43a3d; break;
  case  250:
    l = 0x384ebda39; break;
  case  251:
    l = 0x38832fa34; break;
  case  252:
    l = 0x38b797a30; break;
  case  253:
    l = 0x38ebf7a2c; break;
  case  254:
    l = 0x39204fa27; break;
  case  255:
    l = 0x39549da23; break;
  case  256:
    l = 0x3988e3a1e; break;
  case  257:
    l = 0x39bd1fa1b; break;
  case  258:
    l = 0x39f155a16; break;
  case  259:
    l = 0x3a2581a11; break;
  case  260:
    l = 0x3a59a3a0d; break;
  case  261:
    l = 0x3a8dbda09; break;
  case  262:
    l = 0x3ac1cfa05; break;
  case  263:
    l = 0x3af5d9a00; break;
  case  264:
    l = 0x3b29d99fc; break;
  case  265:
    l = 0x3b5dd19f8; break;
  case  266:
    l = 0x3b91c19f3; break;
  case  267:
    l = 0x3bc5a79f0; break;
  case  268:
    l = 0x3bf9879eb; break;
  case  269:
    l = 0x3c2d5d9e6; break;
  case  270:
    l = 0x3c61299e3; break;
  case  271:
    l = 0x3c94ef9de; break;
  case  272:
    l = 0x3cc8ab9da; break;
  case  273:
    l = 0x3cfc5f9d6; break;
  case  274:
    l = 0x3d300b9d2; break;
  case  275:
    l = 0x3d63af9cd; break;
  case  276:
    l = 0x3d97499c9; break;
  case  277:
    l = 0x3dcadb9c5; break;
  case  278:
    l = 0x3dfe659c1; break;
  case  279:
    l = 0x3e31e79bd; break;
  case  280:
    l = 0x3e65619b8; break;
  case  281:
    l = 0x3e98d19b5; break;
  case  282:
    l = 0x3ecc3b9b0; break;
  case  283:
    l = 0x3eff9b9ac; break;
  case  284:
    l = 0x3f32f39a8; break;
  case  285:
    l = 0x3f66439a4; break;
  case  286:
    l = 0x3f998b9a0; break;
  case  287:
    l = 0x3fcccb99b; break;
  case  288:
    l = 0x400001998; break;
  case  289:
    l = 0x403331993; break;
  case  290:
    l = 0x406657990; break;
  case  291:
    l = 0x40997798b; break;
  case  292:
    l = 0x40cc8d987; break;
  case  293:
    l = 0x40ff9b983; break;
  case  294:
    l = 0x4132a1980; break;
  case  295:
    l = 0x4165a197b; break;
  case  296:
    l = 0x419897977; break;
  case  297:
    l = 0x41cb85973; break;
  case  298:
    l = 0x41fe6b96f; break;
  case  299:
    l = 0x42314996b; break;
  case  300:
    l = 0x42641f967; break;
  case  301:
    l = 0x4296ed963; break;
  case  302:
    l = 0x42c9b395f; break;
  case  303:
    l = 0x42fc7195b; break;
  case  304:
    l = 0x432f27957; break;
  case  305:
    l = 0x4361d5953; break;
  case  306:
    l = 0x43947b94f; break;
  case  307:
    l = 0x43c71994b; break;
  case  308:
    l = 0x43f9af947; break;
  case  309:
    l = 0x442c3d944; break;
  case  310:
    l = 0x445ec593f; break;
  case  311:
    l = 0x44914393b; break;
  case  312:
    l = 0x44c3b9938; break;
  case  313:
    l = 0x44f629933; break;
  case  314:
    l = 0x45288f930; break;
  case  315:
    l = 0x455aef92c; break;
  case  316:
    l = 0x458d47928; break;
  case  317:
    l = 0x45bf97924; break;
  case  318:
    l = 0x45f1df920; break;
  case  319:
    l = 0x46241f91c; break;
  case  320:
    l = 0x465657919; break;
  case  321:
    l = 0x468889914; break;
  case  322:
    l = 0x46bab1911; break;
  case  323:
    l = 0x46ecd390d; break;
  case  324:
    l = 0x471eed909; break;
  case  325:
    l = 0x4750ff905; break;
  case  326:
    l = 0x478309901; break;
  case  327:
    l = 0x47b50b8fe; break;
  case  328:
    l = 0x47e7078fa; break;
  case  329:
    l = 0x4818fb8f6; break;
  case  330:
    l = 0x484ae78f2; break;
  case  331:
    l = 0x487ccb8ee; break;
  case  332:
    l = 0x48aea78eb; break;
  case  333:
    l = 0x48e07d8e7; break;
  case  334:
    l = 0x49124b8e3; break;
  case  335:
    l = 0x4944118df; break;
  case  336:
    l = 0x4975cf8db; break;
  case  337:
    l = 0x49a7858d8; break;
  case  338:
    l = 0x49d9358d4; break;
  case  339:
    l = 0x4a0add8d1; break;
  case  340:
    l = 0x4a3c7f8cc; break;
  case  341:
    l = 0x4a6e178c9; break;
  case  342:
    l = 0x4a9fa98c5; break;
  case  343:
    l = 0x4ad1338c2; break;
  case  344:
    l = 0x4b02b78bd; break;
  case  345:
    l = 0x4b34318bb; break;
  case  346:
    l = 0x4b65a78b6; break;
  case  347:
    l = 0x4b97138b3; break;
  case  348:
    l = 0x4bc8798af; break;
  case  349:
    l = 0x4bf9d78ab; break;
  case  350:
    l = 0x4c2b2d8a8; break;
  case  351:
    l = 0x4c5c7d8a4; break;
  case  352:
    l = 0x4c8dc58a0; break;
  case  353:
    l = 0x4cbf0589d; break;
  case  354:
    l = 0x4cf03f899; break;
  case  355:
    l = 0x4d2171895; break;
  case  356:
    l = 0x4d529b892; break;
  case  357:
    l = 0x4d83bf88e; break;
  case  358:
    l = 0x4db4db88b; break;
  case  359:
    l = 0x4de5f1887; break;
  case  360:
    l = 0x4e16ff883; break;
  case  361:
    l = 0x4e4805880; break;
  case  362:
    l = 0x4e790587d; break;
  case  363:
    l = 0x4ea9ff878; break;
  case  364:
    l = 0x4edaef875; break;
  case  365:
    l = 0x4f0bd9872; break;
  case  366:
    l = 0x4f3cbd86e; break;
  case  367:
    l = 0x4f6d9986a; break;
  case  368:
    l = 0x4f9e6d867; break;
  case  369:
    l = 0x4fcf3b863; break;
  case  370:
    l = 0x500001860; break;
  case  371:
    l = 0x5030c185c; break;
  case  372:
    l = 0x506179859; break;
  case  373:
    l = 0x50922b855; break;
  case  374:
    l = 0x50c2d5852; break;
  case  375:
    l = 0x50f37984e; break;
  case  376:
    l = 0x51241584b; break;
  case  377:
    l = 0x5154ab847; break;
  case  378:
    l = 0x518539843; break;
  case  379:
    l = 0x51b5bf841; break;
  case  380:
    l = 0x51e64183c; break;
  case  381:
    l = 0x5216b9839; break;
  case  382:
    l = 0x52472b836; break;
  case  383:
    l = 0x527797833; break;
  case  384:
    l = 0x52a7fd82e; break;
  case  385:
    l = 0x52d85982c; break;
  case  386:
    l = 0x5308b1828; break;
  case  387:
    l = 0x533901824; break;
  case  388:
    l = 0x536949821; break;
  case  389:
    l = 0x53998b81e; break;
  case  390:
    l = 0x53c9c781a; break;
  case  391:
    l = 0x53f9fb817; break;
  case  392:
    l = 0x542a29813; break;
  case  393:
    l = 0x545a4f810; break;
  case  394:
    l = 0x548a6f80d; break;
  case  395:
    l = 0x54ba89809; break;
  case  396:
    l = 0x54ea9b806; break;
  case  397:
    l = 0x551aa7802; break;
  case  398:
    l = 0x554aab800; break;
  case  399:
    l = 0x557aab7fb; break;
  case  400:
    l = 0x55aaa17f9; break;
  case  401:
    l = 0x55da937f5; break;
  case  402:
    l = 0x560a7d7f1; break;
  case  403:
    l = 0x563a5f7ee; break;
  case  404:
    l = 0x566a3b7eb; break;
  case  405:
    l = 0x569a117e8; break;
  case  406:
    l = 0x56c9e17e4; break;
  case  407:
    l = 0x56f9a97e1; break;
  case  408:
    l = 0x57296b7de; break;
  case  409:
    l = 0x5759277da; break;
  case  410:
    l = 0x5788db7d7; break;
  case  411:
    l = 0x57b8897d4; break;
  case  412:
    l = 0x57e8317d0; break;
  case  413:
    l = 0x5817d17cd; break;
  case  414:
    l = 0x58476b7ca; break;
  case  415:
    l = 0x5876ff7c6; break;
  case  416:
    l = 0x58a68b7c4; break;
  case  417:
    l = 0x58d6137c0; break;
  case  418:
    l = 0x5905937bc; break;
  case  419:
    l = 0x59350b7ba; break;
  case  420:
    l = 0x59647f7b6; break;
  case  421:
    l = 0x5993eb7b3; break;
  case  422:
    l = 0x59c3517af; break;
  case  423:
    l = 0x59f2af7ad; break;
  case  424:
    l = 0x5a22097a9; break;
  case  425:
    l = 0x5a515b7a6; break;
  case  426:
    l = 0x5a80a77a3; break;
  case  427:
    l = 0x5aafed79f; break;
  case  428:
    l = 0x5adf2b79c; break;
  case  429:
    l = 0x5b0e6379a; break;
  case  430:
    l = 0x5b3d97795; break;
  case  431:
    l = 0x5b6cc1793; break;
  case  432:
    l = 0x5b9be7790; break;
  case  433:
    l = 0x5bcb0778c; break;
  case  434:
    l = 0x5bfa1f789; break;
  case  435:
    l = 0x5c2931786; break;
  case  436:
    l = 0x5c583d783; break;
  case  437:
    l = 0x5c874377f; break;
  case  438:
    l = 0x5cb64177d; break;
  case  439:
    l = 0x5ce53b779; break;
  case  440:
    l = 0x5d142d776; break;
  case  441:
    l = 0x5d4319773; break;
  case  442:
    l = 0x5d71ff770; break;
  case  443:
    l = 0x5da0df76c; break;
  case  444:
    l = 0x5dcfb776a; break;
  case  445:
    l = 0x5dfe8b766; break;
  case  446:
    l = 0x5e2d57764; break;
  case  447:
    l = 0x5e5c1f760; break;
  case  448:
    l = 0x5e8adf75d; break;
  case  449:
    l = 0x5eb99975a; break;
  case  450:
    l = 0x5ee84d757; break;
  case  451:
    l = 0x5f16fb753; break;
  case  452:
    l = 0x5f45a1751; break;
  case  453:
    l = 0x5f744374d; break;
  case  454:
    l = 0x5fa2dd74b; break;
  case  455:
    l = 0x5fd173747; break;
  case  456:
    l = 0x600001744; break;
  case  457:
    l = 0x602e89742; break;
  case  458:
    l = 0x605d0d73e; break;
  case  459:
    l = 0x608b8973b; break;
  case  460:
    l = 0x60b9ff738; break;
  case  461:
    l = 0x60e86f735; break;
  case  462:
    l = 0x6116d9732; break;
  case  463:
    l = 0x61453d72f; break;
  case  464:
    l = 0x61739b72b; break;
  case  465:
    l = 0x61a1f1729; break;
  case  466:
    l = 0x61d043726; break;
  case  467:
    l = 0x61fe8f723; break;
  case  468:
    l = 0x622cd5720; break;
  case  469:
    l = 0x625b1571c; break;
  case  470:
    l = 0x62894d71a; break;
  case  471:
    l = 0x62b781717; break;
  case  472:
    l = 0x62e5af713; break;
  case  473:
    l = 0x6313d5711; break;
  case  474:
    l = 0x6341f770e; break;
  case  475:
    l = 0x63701370a; break;
  case  476:
    l = 0x639e27708; break;
  case  477:
    l = 0x63cc37705; break;
  case  478:
    l = 0x63fa41702; break;
  case  479:
    l = 0x6428456fe; break;
  case  480:
    l = 0x6456416fc; break;
  case  481:
    l = 0x6484396f9; break;
  case  482:
    l = 0x64b22b6f6; break;
  case  483:
    l = 0x64e0176f3; break;
  case  484:
    l = 0x650dfd6f0; break;
  case  485:
    l = 0x653bdd6ed; break;
  case  486:
    l = 0x6569b76ea; break;
  case  487:
    l = 0x65978b6e7; break;
  case  488:
    l = 0x65c5596e5; break;
  case  489:
    l = 0x65f3236e1; break;
  case  490:
    l = 0x6620e56de; break;
  case  491:
    l = 0x664ea16dc; break;
  case  492:
    l = 0x667c596d8; break;
  case  493:
    l = 0x66aa096d6; break;
  case  494:
    l = 0x66d7b56d3; break;
  case  495:
    l = 0x67055b6d0; break;
  case  496:
    l = 0x6732fb6cd; break;
  case  497:
    l = 0x6760956ca; break;
  case  498:
    l = 0x678e296c7; break;
  case  499:
    l = 0x67bbb76c4; break;
  case  500:
    l = 0x67e93f6c2; break;
  case  501:
    l = 0x6816c36be; break;
  case  502:
    l = 0x68443f6bc; break;
  case  503:
    l = 0x6871b76b9; break;
  case  504:
    l = 0x689f296b6; break;
  case  505:
    l = 0x68cc956b3; break;
  case  506:
    l = 0x68f9fb6b0; break;
  case  507:
    l = 0x69275b6ad; break;
  case  508:
    l = 0x6954b56ab; break;
  case  509:
    l = 0x69820b6a7; break;
  case  510:
    l = 0x69af596a5; break;
  case  511:
    l = 0x69dca36a2; break;
  case  512:
    l = 0x6a09ead3b; break;
  case  513:
    l = 0x6a6460d31; break;
  case  514:
    l = 0x6abec2d25; break;
  case  515:
    l = 0x6b190cd1a; break;
  case  516:
    l = 0x6b7340d0e; break;
  case  517:
    l = 0x6bcd5cd04; break;
  case  518:
    l = 0x6c2764cf8; break;
  case  519:
    l = 0x6c8154cee; break;
  case  520:
    l = 0x6cdb30ce2; break;
  case  521:
    l = 0x6d34f4cd7; break;
  case  522:
    l = 0x6d8ea2ccc; break;
  case  523:
    l = 0x6de83acc2; break;
  case  524:
    l = 0x6e41becb6; break;
  case  525:
    l = 0x6e9b2acab; break;
  case  526:
    l = 0x6ef480ca1; break;
  case  527:
    l = 0x6f4dc2c96; break;
  case  528:
    l = 0x6fa6eec8b; break;
  case  529:
    l = 0x700004c80; break;
  case  530:
    l = 0x705904c75; break;
  case  531:
    l = 0x70b1eec6b; break;
  case  532:
    l = 0x710ac4c60; break;
  case  533:
    l = 0x716384c55; break;
  case  534:
    l = 0x71bc2ec4b; break;
  case  535:
    l = 0x7214c4c40; break;
  case  536:
    l = 0x726d44c36; break;
  case  537:
    l = 0x72c5b0c2b; break;
  case  538:
    l = 0x731e06c21; break;
  case  539:
    l = 0x737648c16; break;
  case  540:
    l = 0x73ce74c0b; break;
  case  541:
    l = 0x74268ac02; break;
  case  542:
    l = 0x747e8ebf7; break;
  case  543:
    l = 0x74d67cbec; break;
  case  544:
    l = 0x752e54be2; break;
  case  545:
    l = 0x758618bd8; break;
  case  546:
    l = 0x75ddc8bce; break;
  case  547:
    l = 0x763564bc3; break;
  case  548:
    l = 0x768ceabb9; break;
  case  549:
    l = 0x76e45cbaf; break;
  case  550:
    l = 0x773bbaba5; break;
  case  551:
    l = 0x779304b9b; break;
  case  552:
    l = 0x77ea3ab90; break;
  case  553:
    l = 0x78415ab87; break;
  case  554:
    l = 0x789868b7c; break;
  case  555:
    l = 0x78ef60b72; break;
  case  556:
    l = 0x794644b69; break;
  case  557:
    l = 0x799d16b5e; break;
  case  558:
    l = 0x79f3d2b55; break;
  case  559:
    l = 0x7a4a7cb4a; break;
  case  560:
    l = 0x7aa110b41; break;
  case  561:
    l = 0x7af792b37; break;
  case  562:
    l = 0x7b4e00b2d; break;
  case  563:
    l = 0x7ba45ab23; break;
  case  564:
    l = 0x7bfaa0b19; break;
  case  565:
    l = 0x7c50d2b10; break;
  case  566:
    l = 0x7ca6f2b06; break;
  case  567:
    l = 0x7cfcfeafc; break;
  case  568:
    l = 0x7d52f6af2; break;
  case  569:
    l = 0x7da8daae9; break;
  case  570:
    l = 0x7dfeacadf; break;
  case  571:
    l = 0x7e546aad6; break;
  case  572:
    l = 0x7eaa16acc; break;
  case  573:
    l = 0x7effaeac2; break;
  case  574:
    l = 0x7f5532ab9; break;
  case  575:
    l = 0x7faaa4ab0; break;
  case  576:
    l = 0x800004aa6; break;
  case  577:
    l = 0x805550a9c; break;
  case  578:
    l = 0x80aa88a93; break;
  case  579:
    l = 0x80ffaea8a; break;
  case  580:
    l = 0x8154c2a80; break;
  case  581:
    l = 0x81a9c2a77; break;
  case  582:
    l = 0x81feb0a6d; break;
  case  583:
    l = 0x82538aa65; break;
  case  584:
    l = 0x82a854a5b; break;
  case  585:
    l = 0x82fd0aa51; break;
  case  586:
    l = 0x8351aca49; break;
  case  587:
    l = 0x83a63ea3f; break;
  case  588:
    l = 0x83fabca36; break;
  case  589:
    l = 0x844f28a2d; break;
  case  590:
    l = 0x84a382a24; break;
  case  591:
    l = 0x84f7caa1a; break;
  case  592:
    l = 0x854bfea12; break;
  case  593:
    l = 0x85a022a08; break;
  case  594:
    l = 0x85f4329ff; break;
  case  595:
    l = 0x8648309f7; break;
  case  596:
    l = 0x869c1e9ed; break;
  case  597:
    l = 0x86eff89e5; break;
  case  598:
    l = 0x8743c29db; break;
  case  599:
    l = 0x8797789d2; break;
  case  600:
    l = 0x87eb1c9ca; break;
  case  601:
    l = 0x883eb09c0; break;
  case  602:
    l = 0x8892309b8; break;
  case  603:
    l = 0x88e5a09af; break;
  case  604:
    l = 0x8938fe9a6; break;
  case  605:
    l = 0x898c4a99e; break;
  case  606:
    l = 0x89df86994; break;
  case  607:
    l = 0x8a32ae98c; break;
  case  608:
    l = 0x8a85c6983; break;
  case  609:
    l = 0x8ad8cc97a; break;
  case  610:
    l = 0x8b2bc0971; break;
  case  611:
    l = 0x8b7ea2969; break;
  case  612:
    l = 0x8bd174961; break;
  case  613:
    l = 0x8c2436957; break;
  case  614:
    l = 0x8c76e494f; break;
  case  615:
    l = 0x8cc982946; break;
  case  616:
    l = 0x8d1c0e93e; break;
  case  617:
    l = 0x8d6e8a936; break;
  case  618:
    l = 0x8dc0f692c; break;
  case  619:
    l = 0x8e134e924; break;
  case  620:
    l = 0x8e659691c; break;
  case  621:
    l = 0x8eb7ce913; break;
  case  622:
    l = 0x8f09f490b; break;
  case  623:
    l = 0x8f5c0a903; break;
  case  624:
    l = 0x8fae108fa; break;
  case  625:
    l = 0x9000048f1; break;
  case  626:
    l = 0x9051e68e9; break;
  case  627:
    l = 0x90a3b88e1; break;
  case  628:
    l = 0x90f57a8d9; break;
  case  629:
    l = 0x91472c8d0; break;
  case  630:
    l = 0x9198cc8c8; break;
  case  631:
    l = 0x91ea5c8bf; break;
  case  632:
    l = 0x923bda8b8; break;
  case  633:
    l = 0x928d4a8af; break;
  case  634:
    l = 0x92dea88a7; break;
  case  635:
    l = 0x932ff689f; break;
  case  636:
    l = 0x938134896; break;
  case  637:
    l = 0x93d26088f; break;
  case  638:
    l = 0x94237e886; break;
  case  639:
    l = 0x94748a87e; break;
  case  640:
    l = 0x94c586876; break;
  case  641:
    l = 0x95167286e; break;
  case  642:
    l = 0x95674e866; break;
  case  643:
    l = 0x95b81a85e; break;
  case  644:
    l = 0x9608d6856; break;
  case  645:
    l = 0x96598284e; break;
  case  646:
    l = 0x96aa1e846; break;
  case  647:
    l = 0x96faaa83e; break;
  case  648:
    l = 0x974b26836; break;
  case  649:
    l = 0x979b9282e; break;
  case  650:
    l = 0x97ebee826; break;
  case  651:
    l = 0x983c3a81f; break;
  case  652:
    l = 0x988c78816; break;
  case  653:
    l = 0x98dca480f; break;
  case  654:
    l = 0x992cc2806; break;
  case  655:
    l = 0x997cce7ff; break;
  case  656:
    l = 0x99cccc7f7; break;
  case  657:
    l = 0x9a1cba7f0; break;
  case  658:
    l = 0x9a6c9a7e7; break;
  case  659:
    l = 0x9abc687e0; break;
  case  660:
    l = 0x9b0c287d8; break;
  case  661:
    l = 0x9b5bd87d1; break;
  case  662:
    l = 0x9bab7a7c8; break;
  case  663:
    l = 0x9bfb0a7c1; break;
  case  664:
    l = 0x9c4a8c7ba; break;
  case  665:
    l = 0x9c9a007b1; break;
  case  666:
    l = 0x9ce9627aa; break;
  case  667:
    l = 0x9d38b67a3; break;
  case  668:
    l = 0x9d87fc79b; break;
  case  669:
    l = 0x9dd732793; break;
  case  670:
    l = 0x9e265878c; break;
  case  671:
    l = 0x9e7570784; break;
  case  672:
    l = 0x9ec47877c; break;
  case  673:
    l = 0x9f1370776; break;
  case  674:
    l = 0x9f625c76d; break;
  case  675:
    l = 0x9fb136766; break;
  case  676:
    l = 0xa0000275f; break;
  case  677:
    l = 0xa04ec0757; break;
  case  678:
    l = 0xa09d6e750; break;
  case  679:
    l = 0xa0ec0e749; break;
  case  680:
    l = 0xa13aa0741; break;
  case  681:
    l = 0xa1892273a; break;
  case  682:
    l = 0xa1d796732; break;
  case  683:
    l = 0xa225fa72b; break;
  case  684:
    l = 0xa27450724; break;
  case  685:
    l = 0xa2c29871c; break;
  case  686:
    l = 0xa310d0715; break;
  case  687:
    l = 0xa35efa70e; break;
  case  688:
    l = 0xa3ad16706; break;
  case  689:
    l = 0xa3fb22700; break;
  case  690:
    l = 0xa449226f8; break;
  case  691:
    l = 0xa497126f0; break;
  case  692:
    l = 0xa4e4f26ea; break;
  case  693:
    l = 0xa532c66e2; break;
  case  694:
    l = 0xa5808a6dc; break;
  case  695:
    l = 0xa5ce426d4; break;
  case  696:
    l = 0xa61bea6cd; break;
  case  697:
    l = 0xa669846c6; break;
  case  698:
    l = 0xa6b7106be; break;
  case  699:
    l = 0xa7048c6b8; break;
  case  700:
    l = 0xa751fc6b1; break;
  case  701:
    l = 0xa79f5e6a9; break;
  case  702:
    l = 0xa7ecb06a3; break;
  case  703:
    l = 0xa839f669b; break;
  case  704:
    l = 0xa8872c694; break;
  case  705:
    l = 0xa8d45468e; break;
  case  706:
    l = 0xa92170686; break;
  case  707:
    l = 0xa96e7c680; break;
  case  708:
    l = 0xa9bb7c678; break;
  case  709:
    l = 0xaa086c672; break;
  case  710:
    l = 0xaa555066a; break;
  case  711:
    l = 0xaaa224664; break;
  case  712:
    l = 0xaaeeec65d; break;
  case  713:
    l = 0xab3ba6656; break;
  case  714:
    l = 0xab885264f; break;
  case  715:
    l = 0xabd4f0648; break;
  case  716:
    l = 0xac2180642; break;
  case  717:
    l = 0xac6e0463a; break;
  case  718:
    l = 0xacba78634; break;
  case  719:
    l = 0xad06e062d; break;
  case  720:
    l = 0xad533a626; break;
  case  721:
    l = 0xad9f8661f; break;
  case  722:
    l = 0xadebc4619; break;
  case  723:
    l = 0xae37f6612; break;
  case  724:
    l = 0xae841a60b; break;
  case  725:
    l = 0xaed030604; break;
  case  726:
    l = 0xaf1c385fe; break;
  case  727:
    l = 0xaf68345f7; break;
  case  728:
    l = 0xafb4225f0; break;
  case  729:
    l = 0xb000025ea; break;
  case  730:
    l = 0xb04bd65e3; break;
  case  731:
    l = 0xb0979c5dd; break;
  case  732:
    l = 0xb0e3565d5; break;
  case  733:
    l = 0xb12f005d0; break;
  case  734:
    l = 0xb17aa05c8; break;
  case  735:
    l = 0xb1c6305c2; break;
  case  736:
    l = 0xb211b45bc; break;
  case  737:
    l = 0xb25d2c5b5; break;
  case  738:
    l = 0xb2a8965ae; break;
  case  739:
    l = 0xb2f3f25a8; break;
  case  740:
    l = 0xb33f425a1; break;
  case  741:
    l = 0xb38a8459b; break;
  case  742:
    l = 0xb3d5ba595; break;
  case  743:
    l = 0xb420e458d; break;
  case  744:
    l = 0xb46bfe588; break;
  case  745:
    l = 0xb4b70e581; break;
  case  746:
    l = 0xb5021057a; break;
  case  747:
    l = 0xb54d04575; break;
  case  748:
    l = 0xb597ee56d; break;
  case  749:
    l = 0xb5e2c8568; break;
  case  750:
    l = 0xb62d98561; break;
  case  751:
    l = 0xb6785a55a; break;
  case  752:
    l = 0xb6c30e554; break;
  case  753:
    l = 0xb70db654e; break;
  case  754:
    l = 0xb75852548; break;
  case  755:
    l = 0xb7a2e2541; break;
  case  756:
    l = 0xb7ed6453b; break;
  case  757:
    l = 0xb837da535; break;
  case  758:
    l = 0xb8824452e; break;
  case  759:
    l = 0xb8cca0528; break;
  case  760:
    l = 0xb916f0522; break;
  case  761:
    l = 0xb9613451c; break;
  case  762:
    l = 0xb9ab6c515; break;
  case  763:
    l = 0xb9f59650f; break;
  case  764:
    l = 0xba3fb4509; break;
  case  765:
    l = 0xba89c6503; break;
  case  766:
    l = 0xbad3cc4fc; break;
  case  767:
    l = 0xbb1dc44f7; break;
  case  768:
    l = 0xbb67b24f0; break;
  case  769:
    l = 0xbbb1924ea; break;
  case  770:
    l = 0xbbfb664e4; break;
  case  771:
    l = 0xbc452e4de; break;
  case  772:
    l = 0xbc8eea4d7; break;
  case  773:
    l = 0xbcd8984d2; break;
  case  774:
    l = 0xbd223c4cc; break;
  case  775:
    l = 0xbd6bd44c5; break;
  case  776:
    l = 0xbdb55e4bf; break;
  case  777:
    l = 0xbdfedc4ba; break;
  case  778:
    l = 0xbe48504b3; break;
  case  779:
    l = 0xbe91b64ad; break;
  case  780:
    l = 0xbedb104a8; break;
  case  781:
    l = 0xbf24604a1; break;
  case  782:
    l = 0xbf6da249b; break;
  case  783:
    l = 0xbfb6d8495; break;
  case  784:
    l = 0xc00002490; break;
  case  785:
    l = 0xc04922489; break;
  case  786:
    l = 0xc09234483; break;
  case  787:
    l = 0xc0db3a47e; break;
  case  788:
    l = 0xc12436477; break;
  case  789:
    l = 0xc16d24472; break;
  case  790:
    l = 0xc1b60846c; break;
  case  791:
    l = 0xc1fee0466; break;
  case  792:
    l = 0xc247ac45f; break;
  case  793:
    l = 0xc2906a45b; break;
  case  794:
    l = 0xc2d920454; break;
  case  795:
    l = 0xc321c844e; break;
  case  796:
    l = 0xc36a64449; break;
  case  797:
    l = 0xc3b2f6442; break;
  case  798:
    l = 0xc3fb7a43d; break;
  case  799:
    l = 0xc443f4437; break;
  case  800:
    l = 0xc48c62432; break;
  case  801:
    l = 0xc4d4c642b; break;
  case  802:
    l = 0xc51d1c426; break;
  case  803:
    l = 0xc56568420; break;
  case  804:
    l = 0xc5ada841a; break;
  case  805:
    l = 0xc5f5dc415; break;
  case  806:
    l = 0xc63e0640e; break;
  case  807:
    l = 0xc68622409; break;
  case  808:
    l = 0xc6ce34404; break;
  case  809:
    l = 0xc7163c3fd; break;
  case  810:
    l = 0xc75e363f8; break;
  case  811:
    l = 0xc7a6263f3; break;
  case  812:
    l = 0xc7ee0c3ec; break;
  case  813:
    l = 0xc835e43e7; break;
  case  814:
    l = 0xc87db23e2; break;
  case  815:
    l = 0xc8c5763db; break;
  case  816:
    l = 0xc90d2c3d6; break;
  case  817:
    l = 0xc954d83d1; break;
  case  818:
    l = 0xc99c7a3cb; break;
  case  819:
    l = 0xc9e4103c5; break;
  case  820:
    l = 0xca2b9a3c0; break;
  case  821:
    l = 0xca731a3ba; break;
  case  822:
    l = 0xcaba8e3b4; break;
  case  823:
    l = 0xcb01f63af; break;
  case  824:
    l = 0xcb49543aa; break;
  case  825:
    l = 0xcb90a83a4; break;
  case  826:
    l = 0xcbd7f039e; break;
  case  827:
    l = 0xcc1f2c399; break;
  case  828:
    l = 0xcc665e393; break;
  case  829:
    l = 0xccad8438e; break;
  case  830:
    l = 0xccf4a0389; break;
  case  831:
    l = 0xcd3bb2382; break;
  case  832:
    l = 0xcd82b637e; break;
  case  833:
    l = 0xcdc9b2378; break;
  case  834:
    l = 0xce10a2373; break;
  case  835:
    l = 0xce578836d; break;
  case  836:
    l = 0xce9e62367; break;
  case  837:
    l = 0xcee530363; break;
  case  838:
    l = 0xcf2bf635d; break;
  case  839:
    l = 0xcf72b0357; break;
  case  840:
    l = 0xcfb95e352; break;
  case  841:
    l = 0xd0000234d; break;
  case  842:
    l = 0xd0469c347; break;
  case  843:
    l = 0xd08d2a343; break;
  case  844:
    l = 0xd0d3b033c; break;
  case  845:
    l = 0xd11a28338; break;
  case  846:
    l = 0xd16098332; break;
  case  847:
    l = 0xd1a6fc32c; break;
  case  848:
    l = 0xd1ed54328; break;
  case  849:
    l = 0xd233a4322; break;
  case  850:
    l = 0xd279e831d; break;
  case  851:
    l = 0xd2c022317; break;
  case  852:
    l = 0xd30650313; break;
  case  853:
    l = 0xd34c7430e; break;
  case  854:
    l = 0xd39290307; break;
  case  855:
    l = 0xd3d89e303; break;
  case  856:
    l = 0xd41ea42fd; break;
  case  857:
    l = 0xd4649e2f8; break;
  case  858:
    l = 0xd4aa8e2f3; break;
  case  859:
    l = 0xd4f0742ee; break;
  case  860:
    l = 0xd536502e8; break;
  case  861:
    l = 0xd57c202e3; break;
  case  862:
    l = 0xd5c1e62de; break;
  case  863:
    l = 0xd607a22d9; break;
  case  864:
    l = 0xd64d542d4; break;
  case  865:
    l = 0xd692fc2cf; break;
  case  866:
    l = 0xd6d89a2c9; break;
  case  867:
    l = 0xd71e2c2c4; break;
  case  868:
    l = 0xd763b42c0; break;
  case  869:
    l = 0xd7a9342ba; break;
  case  870:
    l = 0xd7eea82b5; break;
  case  871:
    l = 0xd834122af; break;
  case  872:
    l = 0xd879702ab; break;
  case  873:
    l = 0xd8bec62a6; break;
  case  874:
    l = 0xd904122a1; break;
  case  875:
    l = 0xd9495429b; break;
  case  876:
    l = 0xd98e8a297; break;
  case  877:
    l = 0xd9d3b8291; break;
  case  878:
    l = 0xda18da28c; break;
  case  879:
    l = 0xda5df2288; break;
  case  880:
    l = 0xdaa302282; break;
  case  881:
    l = 0xdae80627e; break;
  case  882:
    l = 0xdb2d02278; break;
  case  883:
    l = 0xdb71f2273; break;
  case  884:
    l = 0xdbb6d826f; break;
  case  885:
    l = 0xdbfbb6269; break;
  case  886:
    l = 0xdc4088264; break;
  case  887:
    l = 0xdc8550260; break;
  case  888:
    l = 0xdcca1025a; break;
  case  889:
    l = 0xdd0ec4256; break;
  case  890:
    l = 0xdd5370251; break;
  case  891:
    l = 0xdd981224b; break;
  case  892:
    l = 0xdddca8247; break;
  case  893:
    l = 0xde2136242; break;
  case  894:
    l = 0xde65ba23d; break;
  case  895:
    l = 0xdeaa34238; break;
  case  896:
    l = 0xdeeea4233; break;
  case  897:
    l = 0xdf330a22e; break;
  case  898:
    l = 0xdf776622a; break;
  case  899:
    l = 0xdfbbba224; break;
  case  900:
    l = 0xe00002220; break;
  case  901:
    l = 0xe0444221b; break;
  case  902:
    l = 0xe08878216; break;
  case  903:
    l = 0xe0cca4211; break;
  case  904:
    l = 0xe110c620c; break;
  case  905:
    l = 0xe154de208; break;
  case  906:
    l = 0xe198ee203; break;
  case  907:
    l = 0xe1dcf41fe; break;
  case  908:
    l = 0xe220f01f9; break;
  case  909:
    l = 0xe264e21f4; break;
  case  910:
    l = 0xe2a8ca1f0; break;
  case  911:
    l = 0xe2ecaa1eb; break;
  case  912:
    l = 0xe330801e6; break;
  case  913:
    l = 0xe3744c1e1; break;
  case  914:
    l = 0xe3b80e1dd; break;
  case  915:
    l = 0xe3fbc81d8; break;
  case  916:
    l = 0xe43f781d3; break;
  case  917:
    l = 0xe4831e1ce; break;
  case  918:
    l = 0xe4c6ba1ca; break;
  case  919:
    l = 0xe50a4e1c5; break;
  case  920:
    l = 0xe54dd81c0; break;
  case  921:
    l = 0xe591581bc; break;
  case  922:
    l = 0xe5d4d01b6; break;
  case  923:
    l = 0xe6183c1b3; break;
  case  924:
    l = 0xe65ba21ad; break;
  case  925:
    l = 0xe69efc1a9; break;
  case  926:
    l = 0xe6e24e1a4; break;
  case  927:
    l = 0xe725961a0; break;
  case  928:
    l = 0xe768d619b; break;
  case  929:
    l = 0xe7ac0c196; break;
  case  930:
    l = 0xe7ef38192; break;
  case  931:
    l = 0xe8325c18d; break;
  case  932:
    l = 0xe87576189; break;
  case  933:
    l = 0xe8b888184; break;
  case  934:
    l = 0xe8fb9017f; break;
  case  935:
    l = 0xe93e8e17b; break;
  case  936:
    l = 0xe98184176; break;
  case  937:
    l = 0xe9c470171; break;
  case  938:
    l = 0xea075216e; break;
  case  939:
    l = 0xea4a2e168; break;
  case  940:
    l = 0xea8cfe164; break;
  case  941:
    l = 0xeacfc615f; break;
  case  942:
    l = 0xeb128415b; break;
  case  943:
    l = 0xeb553a156; break;
  case  944:
    l = 0xeb97e6152; break;
  case  945:
    l = 0xebda8a14e; break;
  case  946:
    l = 0xec1d26148; break;
  case  947:
    l = 0xec5fb6145; break;
  case  948:
    l = 0xeca24013f; break;
  case  949:
    l = 0xece4be13c; break;
  case  950:
    l = 0xed2736137; break;
  case  951:
    l = 0xed69a4132; break;
  case  952:
    l = 0xedac0812e; break;
  case  953:
    l = 0xedee64129; break;
  case  954:
    l = 0xee30b6125; break;
  case  955:
    l = 0xee7300121; break;
  case  956:
    l = 0xeeb54211c; break;
  case  957:
    l = 0xeef77a118; break;
  case  958:
    l = 0xef39aa113; break;
  case  959:
    l = 0xef7bd010f; break;
  case  960:
    l = 0xefbdee10a; break;
  case  961:
    l = 0xf00002106; break;
  case  962:
    l = 0xf0420e102; break;
  case  963:
    l = 0xf084120fd; break;
  case  964:
    l = 0xf0c60c0f9; break;
  case  965:
    l = 0xf107fe0f5; break;
  case  966:
    l = 0xf149e80f0; break;
  case  967:
    l = 0xf18bc80ec; break;
  case  968:
    l = 0xf1cda00e7; break;
  case  969:
    l = 0xf20f6e0e3; break;
  case  970:
    l = 0xf251340df; break;
  case  971:
    l = 0xf292f20da; break;
  case  972:
    l = 0xf2d4a60d7; break;
  case  973:
    l = 0xf316540d1; break;
  case  974:
    l = 0xf357f60ce; break;
  case  975:
    l = 0xf399920c9; break;
  case  976:
    l = 0xf3db240c5; break;
  case  977:
    l = 0xf41cae0c0; break;
  case  978:
    l = 0xf45e2e0bd; break;
  case  979:
    l = 0xf49fa80b8; break;
  case  980:
    l = 0xf4e1180b4; break;
  case  981:
    l = 0xf522800af; break;
  case  982:
    l = 0xf563de0ab; break;
  case  983:
    l = 0xf5a5340a7; break;
  case  984:
    l = 0xf5e6820a3; break;
  case  985:
    l = 0xf627c809e; break;
  case  986:
    l = 0xf6690409b; break;
  case  987:
    l = 0xf6aa3a096; break;
  case  988:
    l = 0xf6eb66091; break;
  case  989:
    l = 0xf72c8808e; break;
  case  990:
    l = 0xf76da4089; break;
  case  991:
    l = 0xf7aeb6086; break;
  case  992:
    l = 0xf7efc2081; break;
  case  993:
    l = 0xf830c407c; break;
  case  994:
    l = 0xf871bc079; break;
  case  995:
    l = 0xf8b2ae074; break;
  case  996:
    l = 0xf8f396071; break;
  case  997:
    l = 0xf9347806c; break;
  case  998:
    l = 0xf97550068; break;
  case  999:
    l = 0xf9b620064; break;
  case 1000:
    l = 0xf9f6e805f; break;
  case 1001:
    l = 0xfa37a605c; break;
  case 1002:
    l = 0xfa785e057; break;
  case 1003:
    l = 0xfab90c053; break;
  case 1004:
    l = 0xfaf9b204f; break;
  case 1005:
    l = 0xfb3a5004b; break;
  case 1006:
    l = 0xfb7ae6047; break;
  case 1007:
    l = 0xfbbb74043; break;
  case 1008:
    l = 0xfbfbfa03f; break;
  case 1009:
    l = 0xfc3c7803b; break;
  case 1010:
    l = 0xfc7cee036; break;
  case 1011:
    l = 0xfcbd5a033; break;
  case 1012:
    l = 0xfcfdc002e; break;
  case 1013:
    l = 0xfd3e1c02a; break;
  case 1014:
    l = 0xfd7e70027; break;
  case 1015:
    l = 0xfdbebe022; break;
  case 1016:
    l = 0xfdff0201e; break;
  case 1017:
    l = 0xfe3f3e01a; break;
  case 1018:
    l = 0xfe7f72016; break;
  case 1019:
    l = 0xfebf9e012; break;
  case 1020:
    l = 0xfeffc200e; break;
  case 1021:
    l = 0xff3fde00a; break;
  case 1022:
    l = 0xff7ff2006; break;
  case 1023:
    l = 0xffbffe002; break;
  default:
    l = 0xfffffffff; break; //通過しないはず
  }
  return (l);
}
