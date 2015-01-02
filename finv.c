/* コンパイル時　gcc finv.c -lm */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "def.h"

#define MASK11 8384512  //((1 << 11) - 1) << 12  MAX = 2048
#define MASK12 4095     //仮数部下位12bit

long long unsigned int make_l(int index);

uint32_t finv(uint32_t a_uint32) {
  
  union data_32bit a, result;
  
  a.uint32 = a_uint32;

  if (a.exp == 255 && a.frac != 0) {  // NaN
    result.uint32 = MY_NAN;
  } else if (a.exp == 0 && a.frac != 0) {// 非正規化数
    if (a.sign == 0) {
      result.uint32 = INF;
    } else {
      result.uint32 = NINF;
    }
  } else if (a.uint32 == INF) {
    result.uint32 = ZERO;
  } else if (a.uint32 == NINF) {
    result.uint32 = NZERO;
  } else if (a.uint32 == ZERO) {
    result.uint32 = INF;
  } else if (a.uint32 == NZERO) {
    result.uint32 = NINF;
  } else if (a.frac == 0) {
    int diff;
    result.uint32 = a.uint32;
    if (a.exp >= 127) {
      diff = a.exp - 127;
      result.exp = 127 - diff;
    } else {
      diff = 127 - a.exp;
      result.exp = 127 + diff;
    }
  } else if (a.exp == 254) {
    if (a.sign == 0) {
      result.uint32 = ZERO;
    } else {
      result.uint32 = NZERO;
    }
  } else {
    
    int index;
    long long unsigned int l;
    unsigned int y,d;
    index = (a.frac & MASK11) >> 12;
    l = make_l(index);
    y = (l & (0xffffff << 24)) >> 24;
    d = l & 0xffffff;
    
    result.sign = a.sign;
    result.exp  = 253 - a.exp;
    result.frac = y + ((d * (4096 - (a.frac & MASK12))) >> 12);
  }
  return (result.uint32);
}

long long unsigned int make_l(int index) {
  long long unsigned int l;
  switch (index) {
  case    0:
    l = 0xffe003001ffd; break;
  case    1:
    l = 0xffc00f001ff4; break;
  case    2:
    l = 0xffa023001fec; break;
  case    3:
    l = 0xff803f001fe4; break;
  case    4:
    l = 0xff6063001fdc; break;
  case    5:
    l = 0xff408f001fd4; break;
  case    6:
    l = 0xff20c3001fcc; break;
  case    7:
    l = 0xff00ff001fc4; break;
  case    8:
    l = 0xfee142001fbd; break;
  case    9:
    l = 0xfec18e001fb4; break;
  case   10:
    l = 0xfea1e1001fad; break;
  case   11:
    l = 0xfe823c001fa5; break;
  case   12:
    l = 0xfe629f001f9d; break;
  case   13:
    l = 0xfe430a001f95; break;
  case   14:
    l = 0xfe237d001f8d; break;
  case   15:
    l = 0xfe03f8001f85; break;
  case   16:
    l = 0xfde47a001f7e; break;
  case   17:
    l = 0xfdc504001f76; break;
  case   18:
    l = 0xfda596001f6e; break;
  case   19:
    l = 0xfd8630001f66; break;
  case   20:
    l = 0xfd66d2001f5e; break;
  case   21:
    l = 0xfd477b001f57; break;
  case   22:
    l = 0xfd282c001f4f; break;
  case   23:
    l = 0xfd08e5001f47; break;
  case   24:
    l = 0xfce9a5001f40; break;
  case   25:
    l = 0xfcca6e001f37; break;
  case   26:
    l = 0xfcab3e001f30; break;
  case   27:
    l = 0xfc8c15001f29; break;
  case   28:
    l = 0xfc6cf5001f20; break;
  case   29:
    l = 0xfc4ddc001f19; break;
  case   30:
    l = 0xfc2eca001f12; break;
  case   31:
    l = 0xfc0fc1001f09; break;
  case   32:
    l = 0xfbf0be001f03; break;
  case   33:
    l = 0xfbd1c4001efa; break;
  case   34:
    l = 0xfbb2d1001ef3; break;
  case   35:
    l = 0xfb93e6001eeb; break;
  case   36:
    l = 0xfb7502001ee4; break;
  case   37:
    l = 0xfb5626001edc; break;
  case   38:
    l = 0xfb3752001ed4; break;
  case   39:
    l = 0xfb1885001ecd; break;
  case   40:
    l = 0xfaf9c0001ec5; break;
  case   41:
    l = 0xfadb02001ebe; break;
  case   42:
    l = 0xfabc4b001eb7; break;
  case   43:
    l = 0xfa9d9d001eae; break;
  case   44:
    l = 0xfa7ef5001ea8; break;
  case   45:
    l = 0xfa6056001e9f; break;
  case   46:
    l = 0xfa41bd001e99; break;
  case   47:
    l = 0xfa232c001e91; break;
  case   48:
    l = 0xfa04a3001e89; break;
  case   49:
    l = 0xf9e621001e82; break;
  case   50:
    l = 0xf9c7a7001e7a; break;
  case   51:
    l = 0xf9a934001e73; break;
  case   52:
    l = 0xf98ac8001e6c; break;
  case   53:
    l = 0xf96c64001e64; break;
  case   54:
    l = 0xf94e07001e5d; break;
  case   55:
    l = 0xf92fb2001e55; break;
  case   56:
    l = 0xf91164001e4e; break;
  case   57:
    l = 0xf8f31d001e47; break;
  case   58:
    l = 0xf8d4de001e3f; break;
  case   59:
    l = 0xf8b6a6001e38; break;
  case   60:
    l = 0xf89875001e31; break;
  case   61:
    l = 0xf87a4c001e29; break;
  case   62:
    l = 0xf85c2a001e22; break;
  case   63:
    l = 0xf83e0f001e1b; break;
  case   64:
    l = 0xf81ffc001e13; break;
  case   65:
    l = 0xf801f0001e0c; break;
  case   66:
    l = 0xf7e3eb001e05; break;
  case   67:
    l = 0xf7c5ed001dfe; break;
  case   68:
    l = 0xf7a7f7001df6; break;
  case   69:
    l = 0xf78a08001def; break;
  case   70:
    l = 0xf76c20001de8; break;
  case   71:
    l = 0xf74e3f001de1; break;
  case   72:
    l = 0xf73066001dd9; break;
  case   73:
    l = 0xf71294001dd2; break;
  case   74:
    l = 0xf6f4c9001dcb; break;
  case   75:
    l = 0xf6d705001dc4; break;
  case   76:
    l = 0xf6b948001dbd; break;
  case   77:
    l = 0xf69b93001db5; break;
  case   78:
    l = 0xf67de4001daf; break;
  case   79:
    l = 0xf6603d001da7; break;
  case   80:
    l = 0xf6429d001da0; break;
  case   81:
    l = 0xf62504001d99; break;
  case   82:
    l = 0xf60772001d92; break;
  case   83:
    l = 0xf5e9e8001d8a; break;
  case   84:
    l = 0xf5cc64001d84; break;
  case   85:
    l = 0xf5aee7001d7d; break;
  case   86:
    l = 0xf59172001d75; break;
  case   87:
    l = 0xf57403001d6f; break;
  case   88:
    l = 0xf5569c001d67; break;
  case   89:
    l = 0xf5393c001d60; break;
  case   90:
    l = 0xf51be2001d5a; break;
  case   91:
    l = 0xf4fe90001d52; break;
  case   92:
    l = 0xf4e145001d4b; break;
  case   93:
    l = 0xf4c401001d44; break;
  case   94:
    l = 0xf4a6c3001d3e; break;
  case   95:
    l = 0xf4898d001d36; break;
  case   96:
    l = 0xf46c5e001d2f; break;
  case   97:
    l = 0xf44f35001d29; break;
  case   98:
    l = 0xf43214001d21; break;
  case   99:
    l = 0xf414f9001d1b; break;
  case  100:
    l = 0xf3f7e6001d13; break;
  case  101:
    l = 0xf3dad9001d0d; break;
  case  102:
    l = 0xf3bdd4001d05; break;
  case  103:
    l = 0xf3a0d5001cff; break;
  case  104:
    l = 0xf383dd001cf8; break;
  case  105:
    l = 0xf366ec001cf1; break;
  case  106:
    l = 0xf34a02001cea; break;
  case  107:
    l = 0xf32d1e001ce4; break;
  case  108:
    l = 0xf31042001cdc; break;
  case  109:
    l = 0xf2f36c001cd6; break;
  case  110:
    l = 0xf2d69e001cce; break;
  case  111:
    l = 0xf2b9d6001cc8; break;
  case  112:
    l = 0xf29d15001cc1; break;
  case  113:
    l = 0xf2805b001cba; break;
  case  114:
    l = 0xf263a7001cb4; break;
  case  115:
    l = 0xf246fa001cad; break;
  case  116:
    l = 0xf22a54001ca6; break;
  case  117:
    l = 0xf20db5001c9f; break;
  case  118:
    l = 0xf1f11d001c98; break;
  case  119:
    l = 0xf1d48b001c92; break;
  case  120:
    l = 0xf1b801001c8a; break;
  case  121:
    l = 0xf19b7c001c85; break;
  case  122:
    l = 0xf17eff001c7d; break;
  case  123:
    l = 0xf16288001c77; break;
  case  124:
    l = 0xf14618001c70; break;
  case  125:
    l = 0xf129af001c69; break;
  case  126:
    l = 0xf10d4c001c63; break;
  case  127:
    l = 0xf0f0f1001c5b; break;
  case  128:
    l = 0xf0d49b001c56; break;
  case  129:
    l = 0xf0b84d001c4e; break;
  case  130:
    l = 0xf09c05001c48; break;
  case  131:
    l = 0xf07fc3001c42; break;
  case  132:
    l = 0xf06389001c3a; break;
  case  133:
    l = 0xf04755001c34; break;
  case  134:
    l = 0xf02b27001c2e; break;
  case  135:
    l = 0xf00f01001c26; break;
  case  136:
    l = 0xeff2e0001c21; break;
  case  137:
    l = 0xefd6c7001c19; break;
  case  138:
    l = 0xefbab4001c13; break;
  case  139:
    l = 0xef9ea7001c0d; break;
  case  140:
    l = 0xef82a1001c06; break;
  case  141:
    l = 0xef66a2001bff; break;
  case  142:
    l = 0xef4aa9001bf9; break;
  case  143:
    l = 0xef2eb7001bf2; break;
  case  144:
    l = 0xef12cb001bec; break;
  case  145:
    l = 0xeef6e6001be5; break;
  case  146:
    l = 0xeedb07001bdf; break;
  case  147:
    l = 0xeebf2f001bd8; break;
  case  148:
    l = 0xeea35d001bd2; break;
  case  149:
    l = 0xee8792001bcb; break;
  case  150:
    l = 0xee6bcd001bc5; break;
  case  151:
    l = 0xee500e001bbf; break;
  case  152:
    l = 0xee3457001bb7; break;
  case  153:
    l = 0xee18a5001bb2; break;
  case  154:
    l = 0xedfcfa001bab; break;
  case  155:
    l = 0xede156001ba4; break;
  case  156:
    l = 0xedc5b7001b9f; break;
  case  157:
    l = 0xedaa20001b97; break;
  case  158:
    l = 0xed8e8e001b92; break;
  case  159:
    l = 0xed7303001b8b; break;
  case  160:
    l = 0xed577f001b84; break;
  case  161:
    l = 0xed3c01001b7e; break;
  case  162:
    l = 0xed2089001b78; break;
  case  163:
    l = 0xed0517001b72; break;
  case  164:
    l = 0xece9ac001b6b; break;
  case  165:
    l = 0xecce47001b65; break;
  case  166:
    l = 0xecb2e9001b5e; break;
  case  167:
    l = 0xec9791001b58; break;
  case  168:
    l = 0xec7c3f001b52; break;
  case  169:
    l = 0xec60f3001b4c; break;
  case  170:
    l = 0xec45ae001b45; break;
  case  171:
    l = 0xec2a6f001b3f; break;
  case  172:
    l = 0xec0f37001b38; break;
  case  173:
    l = 0xebf404001b33; break;
  case  174:
    l = 0xebd8d8001b2c; break;
  case  175:
    l = 0xebbdb2001b26; break;
  case  176:
    l = 0xeba293001b1f; break;
  case  177:
    l = 0xeb8779001b1a; break;
  case  178:
    l = 0xeb6c66001b13; break;
  case  179:
    l = 0xeb5159001b0d; break;
  case  180:
    l = 0xeb3653001b06; break;
  case  181:
    l = 0xeb1b52001b01; break;
  case  182:
    l = 0xeb0058001afa; break;
  case  183:
    l = 0xeae564001af4; break;
  case  184:
    l = 0xeaca76001aee; break;
  case  185:
    l = 0xeaaf8e001ae8; break;
  case  186:
    l = 0xea94ac001ae2; break;
  case  187:
    l = 0xea79d1001adb; break;
  case  188:
    l = 0xea5efc001ad5; break;
  case  189:
    l = 0xea442c001ad0; break;
  case  190:
    l = 0xea2963001ac9; break;
  case  191:
    l = 0xea0ea1001ac2; break;
  case  192:
    l = 0xe9f3e4001abd; break;
  case  193:
    l = 0xe9d92d001ab7; break;
  case  194:
    l = 0xe9be7c001ab1; break;
  case  195:
    l = 0xe9a3d2001aaa; break;
  case  196:
    l = 0xe9892e001aa4; break;
  case  197:
    l = 0xe96e8f001a9f; break;
  case  198:
    l = 0xe953f7001a98; break;
  case  199:
    l = 0xe93965001a92; break;
  case  200:
    l = 0xe91ed9001a8c; break;
  case  201:
    l = 0xe90452001a87; break;
  case  202:
    l = 0xe8e9d2001a80; break;
  case  203:
    l = 0xe8cf58001a7a; break;
  case  204:
    l = 0xe8b4e4001a74; break;
  case  205:
    l = 0xe89a76001a6e; break;
  case  206:
    l = 0xe8800e001a68; break;
  case  207:
    l = 0xe865ac001a62; break;
  case  208:
    l = 0xe84b50001a5c; break;
  case  209:
    l = 0xe830fa001a56; break;
  case  210:
    l = 0xe816aa001a50; break;
  case  211:
    l = 0xe7fc60001a4a; break;
  case  212:
    l = 0xe7e21b001a45; break;
  case  213:
    l = 0xe7c7dd001a3e; break;
  case  214:
    l = 0xe7ada5001a38; break;
  case  215:
    l = 0xe79373001a32; break;
  case  216:
    l = 0xe77946001a2d; break;
  case  217:
    l = 0xe75f1f001a27; break;
  case  218:
    l = 0xe744ff001a20; break;
  case  219:
    l = 0xe72ae4001a1b; break;
  case  220:
    l = 0xe710cf001a15; break;
  case  221:
    l = 0xe6f6c0001a0f; break;
  case  222:
    l = 0xe6dcb7001a09; break;
  case  223:
    l = 0xe6c2b4001a03; break;
  case  224:
    l = 0xe6a8b70019fd; break;
  case  225:
    l = 0xe68ebf0019f8; break;
  case  226:
    l = 0xe674cd0019f2; break;
  case  227:
    l = 0xe65ae10019ec; break;
  case  228:
    l = 0xe640fb0019e6; break;
  case  229:
    l = 0xe6271b0019e0; break;
  case  230:
    l = 0xe60d410019da; break;
  case  231:
    l = 0xe5f36c0019d5; break;
  case  232:
    l = 0xe5d99e0019ce; break;
  case  233:
    l = 0xe5bfd50019c9; break;
  case  234:
    l = 0xe5a6110019c4; break;
  case  235:
    l = 0xe58c540019bd; break;
  case  236:
    l = 0xe5729c0019b8; break;
  case  237:
    l = 0xe558ea0019b2; break;
  case  238:
    l = 0xe53f3e0019ac; break;
  case  239:
    l = 0xe525980019a6; break;
  case  240:
    l = 0xe50bf70019a1; break;
  case  241:
    l = 0xe4f25c00199b; break;
  case  242:
    l = 0xe4d8c7001995; break;
  case  243:
    l = 0xe4bf37001990; break;
  case  244:
    l = 0xe4a5ae001989; break;
  case  245:
    l = 0xe48c2a001984; break;
  case  246:
    l = 0xe472ab00197f; break;
  case  247:
    l = 0xe45932001979; break;
  case  248:
    l = 0xe43fbf001973; break;
  case  249:
    l = 0xe4265200196d; break;
  case  250:
    l = 0xe40cea001968; break;
  case  251:
    l = 0xe3f388001962; break;
  case  252:
    l = 0xe3da2c00195c; break;
  case  253:
    l = 0xe3c0d5001957; break;
  case  254:
    l = 0xe3a784001951; break;
  case  255:
    l = 0xe38e3900194b; break;
  case  256:
    l = 0xe374f3001946; break;
  case  257:
    l = 0xe35bb2001941; break;
  case  258:
    l = 0xe3427800193a; break;
  case  259:
    l = 0xe32943001935; break;
  case  260:
    l = 0xe31013001930; break;
  case  261:
    l = 0xe2f6e900192a; break;
  case  262:
    l = 0xe2ddc5001924; break;
  case  263:
    l = 0xe2c4a600191f; break;
  case  264:
    l = 0xe2ab8d001919; break;
  case  265:
    l = 0xe29279001914; break;
  case  266:
    l = 0xe2796b00190e; break;
  case  267:
    l = 0xe26063001908; break;
  case  268:
    l = 0xe24760001903; break;
  case  269:
    l = 0xe22e620018fe; break;
  case  270:
    l = 0xe2156a0018f8; break;
  case  271:
    l = 0xe1fc780018f2; break;
  case  272:
    l = 0xe1e38b0018ed; break;
  case  273:
    l = 0xe1caa30018e8; break;
  case  274:
    l = 0xe1b1c10018e2; break;
  case  275:
    l = 0xe198e50018dc; break;
  case  276:
    l = 0xe1800e0018d7; break;
  case  277:
    l = 0xe1673c0018d2; break;
  case  278:
    l = 0xe14e700018cc; break;
  case  279:
    l = 0xe135a90018c7; break;
  case  280:
    l = 0xe11ce80018c1; break;
  case  281:
    l = 0xe1042c0018bc; break;
  case  282:
    l = 0xe0eb760018b6; break;
  case  283:
    l = 0xe0d2c50018b1; break;
  case  284:
    l = 0xe0ba1a0018ab; break;
  case  285:
    l = 0xe0a1740018a6; break;
  case  286:
    l = 0xe088d30018a1; break;
  case  287:
    l = 0xe0703800189b; break;
  case  288:
    l = 0xe057a2001896; break;
  case  289:
    l = 0xe03f11001891; break;
  case  290:
    l = 0xe0268600188b; break;
  case  291:
    l = 0xe00e01001885; break;
  case  292:
    l = 0xdff580001881; break;
  case  293:
    l = 0xdfdd0500187b; break;
  case  294:
    l = 0xdfc48f001876; break;
  case  295:
    l = 0xdfac1f001870; break;
  case  296:
    l = 0xdf93b400186b; break;
  case  297:
    l = 0xdf7b4e001866; break;
  case  298:
    l = 0xdf62ee001860; break;
  case  299:
    l = 0xdf4a9300185b; break;
  case  300:
    l = 0xdf323d001856; break;
  case  301:
    l = 0xdf19ed001850; break;
  case  302:
    l = 0xdf01a200184b; break;
  case  303:
    l = 0xdee95c001846; break;
  case  304:
    l = 0xded11b001841; break;
  case  305:
    l = 0xdeb8e000183b; break;
  case  306:
    l = 0xdea0aa001836; break;
  case  307:
    l = 0xde8879001831; break;
  case  308:
    l = 0xde704e00182b; break;
  case  309:
    l = 0xde5828001826; break;
  case  310:
    l = 0xde4007001821; break;
  case  311:
    l = 0xde27eb00181c; break;
  case  312:
    l = 0xde0fd4001817; break;
  case  313:
    l = 0xddf7c3001811; break;
  case  314:
    l = 0xdddfb700180c; break;
  case  315:
    l = 0xddc7b0001807; break;
  case  316:
    l = 0xddafae001802; break;
  case  317:
    l = 0xdd97b20017fc; break;
  case  318:
    l = 0xdd7fba0017f8; break;
  case  319:
    l = 0xdd67c80017f2; break;
  case  320:
    l = 0xdd4fdb0017ed; break;
  case  321:
    l = 0xdd37f40017e7; break;
  case  322:
    l = 0xdd20110017e3; break;
  case  323:
    l = 0xdd08330017de; break;
  case  324:
    l = 0xdcf05b0017d8; break;
  case  325:
    l = 0xdcd8880017d3; break;
  case  326:
    l = 0xdcc0ba0017ce; break;
  case  327:
    l = 0xdca8f10017c9; break;
  case  328:
    l = 0xdc912d0017c4; break;
  case  329:
    l = 0xdc796f0017be; break;
  case  330:
    l = 0xdc61b50017ba; break;
  case  331:
    l = 0xdc4a010017b4; break;
  case  332:
    l = 0xdc32510017b0; break;
  case  333:
    l = 0xdc1aa70017aa; break;
  case  334:
    l = 0xdc03020017a5; break;
  case  335:
    l = 0xdbeb620017a0; break;
  case  336:
    l = 0xdbd3c700179b; break;
  case  337:
    l = 0xdbbc31001796; break;
  case  338:
    l = 0xdba4a0001791; break;
  case  339:
    l = 0xdb8d1400178c; break;
  case  340:
    l = 0xdb758d001787; break;
  case  341:
    l = 0xdb5e0b001782; break;
  case  342:
    l = 0xdb468f00177c; break;
  case  343:
    l = 0xdb2f17001778; break;
  case  344:
    l = 0xdb17a4001773; break;
  case  345:
    l = 0xdb003600176e; break;
  case  346:
    l = 0xdae8ce001768; break;
  case  347:
    l = 0xdad16a001764; break;
  case  348:
    l = 0xdaba0b00175f; break;
  case  349:
    l = 0xdaa2b2001759; break;
  case  350:
    l = 0xda8b5d001755; break;
  case  351:
    l = 0xda740d001750; break;
  case  352:
    l = 0xda5cc300174a; break;
  case  353:
    l = 0xda457d001746; break;
  case  354:
    l = 0xda2e3c001741; break;
  case  355:
    l = 0xda170000173c; break;
  case  356:
    l = 0xd9ffc9001737; break;
  case  357:
    l = 0xd9e897001732; break;
  case  358:
    l = 0xd9d16a00172d; break;
  case  359:
    l = 0xd9ba42001728; break;
  case  360:
    l = 0xd9a31f001723; break;
  case  361:
    l = 0xd98c0100171e; break;
  case  362:
    l = 0xd974e700171a; break;
  case  363:
    l = 0xd95dd3001714; break;
  case  364:
    l = 0xd946c3001710; break;
  case  365:
    l = 0xd92fb800170b; break;
  case  366:
    l = 0xd918b3001705; break;
  case  367:
    l = 0xd901b2001701; break;
  case  368:
    l = 0xd8eab60016fc; break;
  case  369:
    l = 0xd8d3be0016f8; break;
  case  370:
    l = 0xd8bccc0016f2; break;
  case  371:
    l = 0xd8a5df0016ed; break;
  case  372:
    l = 0xd88ef60016e9; break;
  case  373:
    l = 0xd878120016e4; break;
  case  374:
    l = 0xd861330016df; break;
  case  375:
    l = 0xd84a590016da; break;
  case  376:
    l = 0xd833840016d5; break;
  case  377:
    l = 0xd81cb40016d0; break;
  case  378:
    l = 0xd805e80016cc; break;
  case  379:
    l = 0xd7ef210016c7; break;
  case  380:
    l = 0xd7d85f0016c2; break;
  case  381:
    l = 0xd7c1a20016bd; break;
  case  382:
    l = 0xd7aae90016b9; break;
  case  383:
    l = 0xd794360016b3; break;
  case  384:
    l = 0xd77d870016af; break;
  case  385:
    l = 0xd766dd0016aa; break;
  case  386:
    l = 0xd750370016a6; break;
  case  387:
    l = 0xd739970016a0; break;
  case  388:
    l = 0xd722fb00169c; break;
  case  389:
    l = 0xd70c64001697; break;
  case  390:
    l = 0xd6f5d1001693; break;
  case  391:
    l = 0xd6df4400168d; break;
  case  392:
    l = 0xd6c8bb001689; break;
  case  393:
    l = 0xd6b237001684; break;
  case  394:
    l = 0xd69bb7001680; break;
  case  395:
    l = 0xd6853c00167b; break;
  case  396:
    l = 0xd66ec6001676; break;
  case  397:
    l = 0xd65855001671; break;
  case  398:
    l = 0xd641e800166d; break;
  case  399:
    l = 0xd62b81001667; break;
  case  400:
    l = 0xd6151d001664; break;
  case  401:
    l = 0xd5febf00165e; break;
  case  402:
    l = 0xd5e86500165a; break;
  case  403:
    l = 0xd5d210001655; break;
  case  404:
    l = 0xd5bbbf001651; break;
  case  405:
    l = 0xd5a57300164c; break;
  case  406:
    l = 0xd58f2c001647; break;
  case  407:
    l = 0xd578e9001643; break;
  case  408:
    l = 0xd562ab00163e; break;
  case  409:
    l = 0xd54c72001639; break;
  case  410:
    l = 0xd5363d001635; break;
  case  411:
    l = 0xd5200d001630; break;
  case  412:
    l = 0xd509e200162b; break;
  case  413:
    l = 0xd4f3bb001627; break;
  case  414:
    l = 0xd4dd98001623; break;
  case  415:
    l = 0xd4c77b00161d; break;
  case  416:
    l = 0xd4b162001619; break;
  case  417:
    l = 0xd49b4d001615; break;
  case  418:
    l = 0xd4853d001610; break;
  case  419:
    l = 0xd46f3200160b; break;
  case  420:
    l = 0xd4592b001607; break;
  case  421:
    l = 0xd44329001602; break;
  case  422:
    l = 0xd42d2b0015fe; break;
  case  423:
    l = 0xd417320015f9; break;
  case  424:
    l = 0xd4013e0015f4; break;
  case  425:
    l = 0xd3eb4e0015f0; break;
  case  426:
    l = 0xd3d5620015ec; break;
  case  427:
    l = 0xd3bf7b0015e7; break;
  case  428:
    l = 0xd3a9990015e2; break;
  case  429:
    l = 0xd393bb0015de; break;
  case  430:
    l = 0xd37de20015d9; break;
  case  431:
    l = 0xd3680d0015d5; break;
  case  432:
    l = 0xd3523d0015d0; break;
  case  433:
    l = 0xd33c710015cc; break;
  case  434:
    l = 0xd326a90015c8; break;
  case  435:
    l = 0xd310e70015c2; break;
  case  436:
    l = 0xd2fb280015bf; break;
  case  437:
    l = 0xd2e56e0015ba; break;
  case  438:
    l = 0xd2cfb90015b5; break;
  case  439:
    l = 0xd2ba080015b1; break;
  case  440:
    l = 0xd2a45b0015ad; break;
  case  441:
    l = 0xd28eb30015a8; break;
  case  442:
    l = 0xd279100015a3; break;
  case  443:
    l = 0xd2637100159f; break;
  case  444:
    l = 0xd24dd600159b; break;
  case  445:
    l = 0xd23840001596; break;
  case  446:
    l = 0xd222ae001592; break;
  case  447:
    l = 0xd20d2100158d; break;
  case  448:
    l = 0xd1f798001589; break;
  case  449:
    l = 0xd1e213001585; break;
  case  450:
    l = 0xd1cc93001580; break;
  case  451:
    l = 0xd1b71700157c; break;
  case  452:
    l = 0xd1a1a0001577; break;
  case  453:
    l = 0xd18c2d001573; break;
  case  454:
    l = 0xd176be00156f; break;
  case  455:
    l = 0xd1615400156a; break;
  case  456:
    l = 0xd14bee001566; break;
  case  457:
    l = 0xd1368d001561; break;
  case  458:
    l = 0xd1213000155d; break;
  case  459:
    l = 0xd10bd7001559; break;
  case  460:
    l = 0xd0f683001554; break;
  case  461:
    l = 0xd0e133001550; break;
  case  462:
    l = 0xd0cbe700154c; break;
  case  463:
    l = 0xd0b6a0001547; break;
  case  464:
    l = 0xd0a15d001543; break;
  case  465:
    l = 0xd08c1e00153f; break;
  case  466:
    l = 0xd076e400153a; break;
  case  467:
    l = 0xd061ae001536; break;
  case  468:
    l = 0xd04c7c001532; break;
  case  469:
    l = 0xd0374e00152e; break;
  case  470:
    l = 0xd02225001529; break;
  case  471:
    l = 0xd00d01001524; break;
  case  472:
    l = 0xcff7e0001521; break;
  case  473:
    l = 0xcfe2c400151c; break;
  case  474:
    l = 0xcfcdac001518; break;
  case  475:
    l = 0xcfb898001514; break;
  case  476:
    l = 0xcfa38900150f; break;
  case  477:
    l = 0xcf8e7e00150b; break;
  case  478:
    l = 0xcf7977001507; break;
  case  479:
    l = 0xcf6474001503; break;
  case  480:
    l = 0xcf4f760014fe; break;
  case  481:
    l = 0xcf3a7c0014fa; break;
  case  482:
    l = 0xcf25860014f6; break;
  case  483:
    l = 0xcf10950014f1; break;
  case  484:
    l = 0xcefba70014ee; break;
  case  485:
    l = 0xcee6be0014e9; break;
  case  486:
    l = 0xced1d90014e5; break;
  case  487:
    l = 0xcebcf80014e1; break;
  case  488:
    l = 0xcea81c0014dc; break;
  case  489:
    l = 0xce93440014d8; break;
  case  490:
    l = 0xce7e700014d4; break;
  case  491:
    l = 0xce69a00014d0; break;
  case  492:
    l = 0xce54d40014cc; break;
  case  493:
    l = 0xce400d0014c7; break;
  case  494:
    l = 0xce2b490014c4; break;
  case  495:
    l = 0xce168a0014bf; break;
  case  496:
    l = 0xce01cf0014bb; break;
  case  497:
    l = 0xcded180014b7; break;
  case  498:
    l = 0xcdd8660014b2; break;
  case  499:
    l = 0xcdc3b70014af; break;
  case  500:
    l = 0xcdaf0d0014aa; break;
  case  501:
    l = 0xcd9a670014a6; break;
  case  502:
    l = 0xcd85c50014a2; break;
  case  503:
    l = 0xcd712700149e; break;
  case  504:
    l = 0xcd5c8d00149a; break;
  case  505:
    l = 0xcd47f8001495; break;
  case  506:
    l = 0xcd3366001492; break;
  case  507:
    l = 0xcd1ed900148d; break;
  case  508:
    l = 0xcd0a50001489; break;
  case  509:
    l = 0xccf5cb001485; break;
  case  510:
    l = 0xcce149001482; break;
  case  511:
    l = 0xcccccd00147c; break;
  case  512:
    l = 0xccb854001479; break;
  case  513:
    l = 0xcca3df001475; break;
  case  514:
    l = 0xcc8f6e001471; break;
  case  515:
    l = 0xcc7b0200146c; break;
  case  516:
    l = 0xcc6699001469; break;
  case  517:
    l = 0xcc5235001464; break;
  case  518:
    l = 0xcc3dd4001461; break;
  case  519:
    l = 0xcc297800145c; break;
  case  520:
    l = 0xcc1520001458; break;
  case  521:
    l = 0xcc00cc001454; break;
  case  522:
    l = 0xcbec7c001450; break;
  case  523:
    l = 0xcbd83000144c; break;
  case  524:
    l = 0xcbc3e7001449; break;
  case  525:
    l = 0xcbafa3001444; break;
  case  526:
    l = 0xcb9b63001440; break;
  case  527:
    l = 0xcb872800143b; break;
  case  528:
    l = 0xcb72f0001438; break;
  case  529:
    l = 0xcb5ebc001434; break;
  case  530:
    l = 0xcb4a8c001430; break;
  case  531:
    l = 0xcb366000142c; break;
  case  532:
    l = 0xcb2238001428; break;
  case  533:
    l = 0xcb0e14001424; break;
  case  534:
    l = 0xcaf9f4001420; break;
  case  535:
    l = 0xcae5d800141c; break;
  case  536:
    l = 0xcad1c0001418; break;
  case  537:
    l = 0xcabdac001414; break;
  case  538:
    l = 0xcaa99c001410; break;
  case  539:
    l = 0xca959000140c; break;
  case  540:
    l = 0xca8188001408; break;
  case  541:
    l = 0xca6d84001404; break;
  case  542:
    l = 0xca5984001400; break;
  case  543:
    l = 0xca45880013fc; break;
  case  544:
    l = 0xca318f0013f9; break;
  case  545:
    l = 0xca1d9b0013f4; break;
  case  546:
    l = 0xca09ab0013f0; break;
  case  547:
    l = 0xc9f5be0013ed; break;
  case  548:
    l = 0xc9e1d60013e8; break;
  case  549:
    l = 0xc9cdf10013e5; break;
  case  550:
    l = 0xc9ba110013e0; break;
  case  551:
    l = 0xc9a6340013dd; break;
  case  552:
    l = 0xc9925b0013d9; break;
  case  553:
    l = 0xc97e860013d5; break;
  case  554:
    l = 0xc96ab50013d1; break;
  case  555:
    l = 0xc956e80013cd; break;
  case  556:
    l = 0xc9431f0013c9; break;
  case  557:
    l = 0xc92f590013c6; break;
  case  558:
    l = 0xc91b980013c1; break;
  case  559:
    l = 0xc907da0013be; break;
  case  560:
    l = 0xc8f4200013ba; break;
  case  561:
    l = 0xc8e06a0013b6; break;
  case  562:
    l = 0xc8ccb80013b2; break;
  case  563:
    l = 0xc8b90a0013ae; break;
  case  564:
    l = 0xc8a5600013aa; break;
  case  565:
    l = 0xc891ba0013a6; break;
  case  566:
    l = 0xc87e170013a3; break;
  case  567:
    l = 0xc86a7800139f; break;
  case  568:
    l = 0xc856dd00139b; break;
  case  569:
    l = 0xc84346001397; break;
  case  570:
    l = 0xc82fb3001393; break;
  case  571:
    l = 0xc81c2400138f; break;
  case  572:
    l = 0xc8089800138c; break;
  case  573:
    l = 0xc7f510001388; break;
  case  574:
    l = 0xc7e18c001384; break;
  case  575:
    l = 0xc7ce0c001380; break;
  case  576:
    l = 0xc7ba9000137c; break;
  case  577:
    l = 0xc7a717001379; break;
  case  578:
    l = 0xc793a3001374; break;
  case  579:
    l = 0xc78032001371; break;
  case  580:
    l = 0xc76cc400136e; break;
  case  581:
    l = 0xc7595b001369; break;
  case  582:
    l = 0xc745f5001366; break;
  case  583:
    l = 0xc73294001361; break;
  case  584:
    l = 0xc71f3600135e; break;
  case  585:
    l = 0xc70bdb00135b; break;
  case  586:
    l = 0xc6f885001356; break;
  case  587:
    l = 0xc6e532001353; break;
  case  588:
    l = 0xc6d1e300134f; break;
  case  589:
    l = 0xc6be9800134b; break;
  case  590:
    l = 0xc6ab50001348; break;
  case  591:
    l = 0xc6980c001344; break;
  case  592:
    l = 0xc684cc001340; break;
  case  593:
    l = 0xc6719000133c; break;
  case  594:
    l = 0xc65e57001339; break;
  case  595:
    l = 0xc64b22001335; break;
  case  596:
    l = 0xc637f1001331; break;
  case  597:
    l = 0xc624c400132d; break;
  case  598:
    l = 0xc6119a00132a; break;
  case  599:
    l = 0xc5fe74001326; break;
  case  600:
    l = 0xc5eb51001323; break;
  case  601:
    l = 0xc5d83300131e; break;
  case  602:
    l = 0xc5c51800131b; break;
  case  603:
    l = 0xc5b201001317; break;
  case  604:
    l = 0xc59eed001314; break;
  case  605:
    l = 0xc58bdd001310; break;
  case  606:
    l = 0xc578d100130c; break;
  case  607:
    l = 0xc565c8001309; break;
  case  608:
    l = 0xc552c3001305; break;
  case  609:
    l = 0xc53fc2001301; break;
  case  610:
    l = 0xc52cc50012fd; break;
  case  611:
    l = 0xc519cb0012fa; break;
  case  612:
    l = 0xc506d40012f7; break;
  case  613:
    l = 0xc4f3e20012f2; break;
  case  614:
    l = 0xc4e0f30012ef; break;
  case  615:
    l = 0xc4ce070012ec; break;
  case  616:
    l = 0xc4bb200012e7; break;
  case  617:
    l = 0xc4a83c0012e4; break;
  case  618:
    l = 0xc4955b0012e1; break;
  case  619:
    l = 0xc4827e0012dd; break;
  case  620:
    l = 0xc46fa50012d9; break;
  case  621:
    l = 0xc45cd00012d5; break;
  case  622:
    l = 0xc449fe0012d2; break;
  case  623:
    l = 0xc4372f0012cf; break;
  case  624:
    l = 0xc424650012ca; break;
  case  625:
    l = 0xc4119d0012c8; break;
  case  626:
    l = 0xc3feda0012c3; break;
  case  627:
    l = 0xc3ec1a0012c0; break;
  case  628:
    l = 0xc3d95d0012bd; break;
  case  629:
    l = 0xc3c6a50012b8; break;
  case  630:
    l = 0xc3b3ef0012b6; break;
  case  631:
    l = 0xc3a13e0012b1; break;
  case  632:
    l = 0xc38e900012ae; break;
  case  633:
    l = 0xc37be50012ab; break;
  case  634:
    l = 0xc3693e0012a7; break;
  case  635:
    l = 0xc3569b0012a3; break;
  case  636:
    l = 0xc343fb0012a0; break;
  case  637:
    l = 0xc3315f00129c; break;
  case  638:
    l = 0xc31ec6001299; break;
  case  639:
    l = 0xc30c31001295; break;
  case  640:
    l = 0xc2f99f001292; break;
  case  641:
    l = 0xc2e71100128e; break;
  case  642:
    l = 0xc2d48600128b; break;
  case  643:
    l = 0xc2c1ff001287; break;
  case  644:
    l = 0xc2af7b001284; break;
  case  645:
    l = 0xc29cfb001280; break;
  case  646:
    l = 0xc28a7f00127c; break;
  case  647:
    l = 0xc27806001279; break;
  case  648:
    l = 0xc26590001276; break;
  case  649:
    l = 0xc2531e001272; break;
  case  650:
    l = 0xc240b000126e; break;
  case  651:
    l = 0xc22e4500126b; break;
  case  652:
    l = 0xc21bdd001268; break;
  case  653:
    l = 0xc20979001264; break;
  case  654:
    l = 0xc1f719001260; break;
  case  655:
    l = 0xc1e4bc00125d; break;
  case  656:
    l = 0xc1d26200125a; break;
  case  657:
    l = 0xc1c00c001256; break;
  case  658:
    l = 0xc1adb9001253; break;
  case  659:
    l = 0xc19b6a00124f; break;
  case  660:
    l = 0xc1891e00124c; break;
  case  661:
    l = 0xc176d6001248; break;
  case  662:
    l = 0xc16491001245; break;
  case  663:
    l = 0xc15250001241; break;
  case  664:
    l = 0xc1401200123e; break;
  case  665:
    l = 0xc12dd700123b; break;
  case  666:
    l = 0xc11ba0001237; break;
  case  667:
    l = 0xc1096d001233; break;
  case  668:
    l = 0xc0f73d001230; break;
  case  669:
    l = 0xc0e51000122d; break;
  case  670:
    l = 0xc0d2e600122a; break;
  case  671:
    l = 0xc0c0c1001225; break;
  case  672:
    l = 0xc0ae9e001223; break;
  case  673:
    l = 0xc09c7f00121f; break;
  case  674:
    l = 0xc08a6300121c; break;
  case  675:
    l = 0xc0784b001218; break;
  case  676:
    l = 0xc06636001215; break;
  case  677:
    l = 0xc05425001211; break;
  case  678:
    l = 0xc0421700120e; break;
  case  679:
    l = 0xc0300c00120b; break;
  case  680:
    l = 0xc01e04001208; break;
  case  681:
    l = 0xc00c01001203; break;
  case  682:
    l = 0xbffa00001201; break;
  case  683:
    l = 0xbfe8030011fd; break;
  case  684:
    l = 0xbfd6090011fa; break;
  case  685:
    l = 0xbfc4130011f6; break;
  case  686:
    l = 0xbfb21f0011f4; break;
  case  687:
    l = 0xbfa0300011ef; break;
  case  688:
    l = 0xbf8e430011ed; break;
  case  689:
    l = 0xbf7c5a0011e9; break;
  case  690:
    l = 0xbf6a750011e5; break;
  case  691:
    l = 0xbf58920011e3; break;
  case  692:
    l = 0xbf46b30011df; break;
  case  693:
    l = 0xbf34d80011db; break;
  case  694:
    l = 0xbf22ff0011d9; break;
  case  695:
    l = 0xbf112a0011d5; break;
  case  696:
    l = 0xbeff590011d1; break;
  case  697:
    l = 0xbeed8a0011cf; break;
  case  698:
    l = 0xbedbbf0011cb; break;
  case  699:
    l = 0xbec9f80011c7; break;
  case  700:
    l = 0xbeb8330011c5; break;
  case  701:
    l = 0xbea6720011c1; break;
  case  702:
    l = 0xbe94b40011be; break;
  case  703:
    l = 0xbe82fa0011ba; break;
  case  704:
    l = 0xbe71430011b7; break;
  case  705:
    l = 0xbe5f8f0011b4; break;
  case  706:
    l = 0xbe4dde0011b1; break;
  case  707:
    l = 0xbe3c310011ad; break;
  case  708:
    l = 0xbe2a870011aa; break;
  case  709:
    l = 0xbe18e00011a7; break;
  case  710:
    l = 0xbe073d0011a3; break;
  case  711:
    l = 0xbdf59c0011a1; break;
  case  712:
    l = 0xbde3ff00119d; break;
  case  713:
    l = 0xbdd266001199; break;
  case  714:
    l = 0xbdc0cf001197; break;
  case  715:
    l = 0xbdaf3c001193; break;
  case  716:
    l = 0xbd9dac001190; break;
  case  717:
    l = 0xbd8c2000118c; break;
  case  718:
    l = 0xbd7a9600118a; break;
  case  719:
    l = 0xbd6910001186; break;
  case  720:
    l = 0xbd578d001183; break;
  case  721:
    l = 0xbd460d001180; break;
  case  722:
    l = 0xbd349100117c; break;
  case  723:
    l = 0xbd2318001179; break;
  case  724:
    l = 0xbd11a2001176; break;
  case  725:
    l = 0xbd002f001173; break;
  case  726:
    l = 0xbceebf001170; break;
  case  727:
    l = 0xbcdd5300116c; break;
  case  728:
    l = 0xbccbea001169; break;
  case  729:
    l = 0xbcba84001166; break;
  case  730:
    l = 0xbca921001163; break;
  case  731:
    l = 0xbc97c200115f; break;
  case  732:
    l = 0xbc866600115c; break;
  case  733:
    l = 0xbc750c00115a; break;
  case  734:
    l = 0xbc63b7001155; break;
  case  735:
    l = 0xbc5264001153; break;
  case  736:
    l = 0xbc4114001150; break;
  case  737:
    l = 0xbc2fc800114c; break;
  case  738:
    l = 0xbc1e7f001149; break;
  case  739:
    l = 0xbc0d39001146; break;
  case  740:
    l = 0xbbfbf6001143; break;
  case  741:
    l = 0xbbeab6001140; break;
  case  742:
    l = 0xbbd97a00113c; break;
  case  743:
    l = 0xbbc84000113a; break;
  case  744:
    l = 0xbbb70a001136; break;
  case  745:
    l = 0xbba5d7001133; break;
  case  746:
    l = 0xbb94a7001130; break;
  case  747:
    l = 0xbb837a00112d; break;
  case  748:
    l = 0xbb7251001129; break;
  case  749:
    l = 0xbb612a001127; break;
  case  750:
    l = 0xbb5007001123; break;
  case  751:
    l = 0xbb3ee7001120; break;
  case  752:
    l = 0xbb2dca00111d; break;
  case  753:
    l = 0xbb1cb000111a; break;
  case  754:
    l = 0xbb0b99001117; break;
  case  755:
    l = 0xbafa85001114; break;
  case  756:
    l = 0xbae975001110; break;
  case  757:
    l = 0xbad86700110e; break;
  case  758:
    l = 0xbac75d00110a; break;
  case  759:
    l = 0xbab656001107; break;
  case  760:
    l = 0xbaa552001104; break;
  case  761:
    l = 0xba9451001101; break;
  case  762:
    l = 0xba83530010fe; break;
  case  763:
    l = 0xba72580010fb; break;
  case  764:
    l = 0xba61600010f8; break;
  case  765:
    l = 0xba506c0010f4; break;
  case  766:
    l = 0xba3f7a0010f2; break;
  case  767:
    l = 0xba2e8b0010ef; break;
  case  768:
    l = 0xba1da00010eb; break;
  case  769:
    l = 0xba0cb80010e8; break;
  case  770:
    l = 0xb9fbd20010e6; break;
  case  771:
    l = 0xb9eaf00010e2; break;
  case  772:
    l = 0xb9da110010df; break;
  case  773:
    l = 0xb9c9350010dc; break;
  case  774:
    l = 0xb9b85c0010d9; break;
  case  775:
    l = 0xb9a7860010d6; break;
  case  776:
    l = 0xb996b30010d3; break;
  case  777:
    l = 0xb985e30010d0; break;
  case  778:
    l = 0xb975160010cd; break;
  case  779:
    l = 0xb9644d0010c9; break;
  case  780:
    l = 0xb953860010c7; break;
  case  781:
    l = 0xb942c20010c4; break;
  case  782:
    l = 0xb932010010c1; break;
  case  783:
    l = 0xb921440010bd; break;
  case  784:
    l = 0xb910890010bb; break;
  case  785:
    l = 0xb8ffd20010b7; break;
  case  786:
    l = 0xb8ef1d0010b5; break;
  case  787:
    l = 0xb8de6b0010b2; break;
  case  788:
    l = 0xb8cdbd0010ae; break;
  case  789:
    l = 0xb8bd110010ac; break;
  case  790:
    l = 0xb8ac690010a8; break;
  case  791:
    l = 0xb89bc30010a6; break;
  case  792:
    l = 0xb88b210010a2; break;
  case  793:
    l = 0xb87a810010a0; break;
  case  794:
    l = 0xb869e500109c; break;
  case  795:
    l = 0xb8594b00109a; break;
  case  796:
    l = 0xb848b4001097; break;
  case  797:
    l = 0xb83821001093; break;
  case  798:
    l = 0xb82790001091; break;
  case  799:
    l = 0xb8170300108d; break;
  case  800:
    l = 0xb8067800108b; break;
  case  801:
    l = 0xb7f5f0001088; break;
  case  802:
    l = 0xb7e56c001084; break;
  case  803:
    l = 0xb7d4ea001082; break;
  case  804:
    l = 0xb7c46b00107f; break;
  case  805:
    l = 0xb7b3ef00107c; break;
  case  806:
    l = 0xb7a376001079; break;
  case  807:
    l = 0xb79301001075; break;
  case  808:
    l = 0xb7828e001073; break;
  case  809:
    l = 0xb7721e001070; break;
  case  810:
    l = 0xb761b000106e; break;
  case  811:
    l = 0xb7514600106a; break;
  case  812:
    l = 0xb740df001067; break;
  case  813:
    l = 0xb7307b001064; break;
  case  814:
    l = 0xb7201a001061; break;
  case  815:
    l = 0xb70fbb00105f; break;
  case  816:
    l = 0xb6ff6000105b; break;
  case  817:
    l = 0xb6ef07001059; break;
  case  818:
    l = 0xb6deb2001055; break;
  case  819:
    l = 0xb6ce5f001053; break;
  case  820:
    l = 0xb6be0f001050; break;
  case  821:
    l = 0xb6adc200104d; break;
  case  822:
    l = 0xb69d7800104a; break;
  case  823:
    l = 0xb68d31001047; break;
  case  824:
    l = 0xb67ced001044; break;
  case  825:
    l = 0xb66cac001041; break;
  case  826:
    l = 0xb65c6d00103f; break;
  case  827:
    l = 0xb64c3200103b; break;
  case  828:
    l = 0xb63bf9001039; break;
  case  829:
    l = 0xb62bc3001036; break;
  case  830:
    l = 0xb61b90001033; break;
  case  831:
    l = 0xb60b6100102f; break;
  case  832:
    l = 0xb5fb3300102e; break;
  case  833:
    l = 0xb5eb0900102a; break;
  case  834:
    l = 0xb5dae2001027; break;
  case  835:
    l = 0xb5cabd001025; break;
  case  836:
    l = 0xb5ba9c001021; break;
  case  837:
    l = 0xb5aa7d00101f; break;
  case  838:
    l = 0xb59a6100101c; break;
  case  839:
    l = 0xb58a48001019; break;
  case  840:
    l = 0xb57a32001016; break;
  case  841:
    l = 0xb56a1f001013; break;
  case  842:
    l = 0xb55a0e001011; break;
  case  843:
    l = 0xb54a0100100d; break;
  case  844:
    l = 0xb539f600100b; break;
  case  845:
    l = 0xb529ee001008; break;
  case  846:
    l = 0xb519e9001005; break;
  case  847:
    l = 0xb509e6001003; break;
  case  848:
    l = 0xb4f9e7000fff; break;
  case  849:
    l = 0xb4e9ea000ffd; break;
  case  850:
    l = 0xb4d9f0000ffa; break;
  case  851:
    l = 0xb4c9f9000ff7; break;
  case  852:
    l = 0xb4ba05000ff4; break;
  case  853:
    l = 0xb4aa14000ff1; break;
  case  854:
    l = 0xb49a25000fef; break;
  case  855:
    l = 0xb48a3a000feb; break;
  case  856:
    l = 0xb47a51000fe9; break;
  case  857:
    l = 0xb46a6b000fe6; break;
  case  858:
    l = 0xb45a87000fe4; break;
  case  859:
    l = 0xb44aa7000fe0; break;
  case  860:
    l = 0xb43ac9000fde; break;
  case  861:
    l = 0xb42aee000fdb; break;
  case  862:
    l = 0xb41b16000fd8; break;
  case  863:
    l = 0xb40b41000fd5; break;
  case  864:
    l = 0xb3fb6e000fd3; break;
  case  865:
    l = 0xb3eb9e000fd0; break;
  case  866:
    l = 0xb3dbd1000fcd; break;
  case  867:
    l = 0xb3cc07000fca; break;
  case  868:
    l = 0xb3bc3f000fc8; break;
  case  869:
    l = 0xb3ac7b000fc4; break;
  case  870:
    l = 0xb39cb9000fc2; break;
  case  871:
    l = 0xb38cfa000fbf; break;
  case  872:
    l = 0xb37d3d000fbd; break;
  case  873:
    l = 0xb36d83000fba; break;
  case  874:
    l = 0xb35dcd000fb6; break;
  case  875:
    l = 0xb34e18000fb5; break;
  case  876:
    l = 0xb33e67000fb1; break;
  case  877:
    l = 0xb32eb8000faf; break;
  case  878:
    l = 0xb31f0c000fac; break;
  case  879:
    l = 0xb30f63000fa9; break;
  case  880:
    l = 0xb2ffbd000fa6; break;
  case  881:
    l = 0xb2f019000fa4; break;
  case  882:
    l = 0xb2e078000fa1; break;
  case  883:
    l = 0xb2d0da000f9e; break;
  case  884:
    l = 0xb2c13e000f9c; break;
  case  885:
    l = 0xb2b1a5000f99; break;
  case  886:
    l = 0xb2a20f000f96; break;
  case  887:
    l = 0xb2927c000f93; break;
  case  888:
    l = 0xb282eb000f91; break;
  case  889:
    l = 0xb2735d000f8e; break;
  case  890:
    l = 0xb263d2000f8b; break;
  case  891:
    l = 0xb2544a000f88; break;
  case  892:
    l = 0xb244c4000f86; break;
  case  893:
    l = 0xb23541000f83; break;
  case  894:
    l = 0xb225c0000f81; break;
  case  895:
    l = 0xb21643000f7d; break;
  case  896:
    l = 0xb206c8000f7b; break;
  case  897:
    l = 0xb1f74f000f79; break;
  case  898:
    l = 0xb1e7da000f75; break;
  case  899:
    l = 0xb1d867000f73; break;
  case  900:
    l = 0xb1c8f6000f71; break;
  case  901:
    l = 0xb1b989000f6d; break;
  case  902:
    l = 0xb1aa1e000f6b; break;
  case  903:
    l = 0xb19ab6000f68; break;
  case  904:
    l = 0xb18b50000f66; break;
  case  905:
    l = 0xb17bed000f63; break;
  case  906:
    l = 0xb16c8d000f60; break;
  case  907:
    l = 0xb15d2f000f5e; break;
  case  908:
    l = 0xb14dd4000f5b; break;
  case  909:
    l = 0xb13e7c000f58; break;
  case  910:
    l = 0xb12f27000f55; break;
  case  911:
    l = 0xb11fd4000f53; break;
  case  912:
    l = 0xb11083000f51; break;
  case  913:
    l = 0xb10136000f4d; break;
  case  914:
    l = 0xb0f1eb000f4b; break;
  case  915:
    l = 0xb0e2a2000f49; break;
  case  916:
    l = 0xb0d35c000f46; break;
  case  917:
    l = 0xb0c419000f43; break;
  case  918:
    l = 0xb0b4d9000f40; break;
  case  919:
    l = 0xb0a59b000f3e; break;
  case  920:
    l = 0xb09660000f3b; break;
  case  921:
    l = 0xb08727000f39; break;
  case  922:
    l = 0xb077f1000f36; break;
  case  923:
    l = 0xb068be000f33; break;
  case  924:
    l = 0xb0598d000f31; break;
  case  925:
    l = 0xb04a5f000f2e; break;
  case  926:
    l = 0xb03b34000f2b; break;
  case  927:
    l = 0xb02c0b000f29; break;
  case  928:
    l = 0xb01ce5000f26; break;
  case  929:
    l = 0xb00dc1000f24; break;
  case  930:
    l = 0xaffea0000f21; break;
  case  931:
    l = 0xafef81000f1f; break;
  case  932:
    l = 0xafe066000f1b; break;
  case  933:
    l = 0xafd14c000f1a; break;
  case  934:
    l = 0xafc236000f16; break;
  case  935:
    l = 0xafb321000f15; break;
  case  936:
    l = 0xafa410000f11; break;
  case  937:
    l = 0xaf9501000f0f; break;
  case  938:
    l = 0xaf85f5000f0c; break;
  case  939:
    l = 0xaf76eb000f0a; break;
  case  940:
    l = 0xaf67e4000f07; break;
  case  941:
    l = 0xaf58df000f05; break;
  case  942:
    l = 0xaf49dd000f02; break;
  case  943:
    l = 0xaf3ade000eff; break;
  case  944:
    l = 0xaf2be1000efd; break;
  case  945:
    l = 0xaf1ce6000efb; break;
  case  946:
    l = 0xaf0def000ef7; break;
  case  947:
    l = 0xaefef9000ef6; break;
  case  948:
    l = 0xaef007000ef2; break;
  case  949:
    l = 0xaee117000ef0; break;
  case  950:
    l = 0xaed229000eee; break;
  case  951:
    l = 0xaec33e000eeb; break;
  case  952:
    l = 0xaeb455000ee9; break;
  case  953:
    l = 0xaea570000ee5; break;
  case  954:
    l = 0xae968c000ee4; break;
  case  955:
    l = 0xae87ab000ee1; break;
  case  956:
    l = 0xae78cd000ede; break;
  case  957:
    l = 0xae69f1000edc; break;
  case  958:
    l = 0xae5b18000ed9; break;
  case  959:
    l = 0xae4c41000ed7; break;
  case  960:
    l = 0xae3d6d000ed4; break;
  case  961:
    l = 0xae2e9b000ed2; break;
  case  962:
    l = 0xae1fcc000ecf; break;
  case  963:
    l = 0xae1100000ecc; break;
  case  964:
    l = 0xae0235000ecb; break;
  case  965:
    l = 0xadf36e000ec7; break;
  case  966:
    l = 0xade4a9000ec5; break;
  case  967:
    l = 0xadd5e6000ec3; break;
  case  968:
    l = 0xadc726000ec0; break;
  case  969:
    l = 0xadb868000ebe; break;
  case  970:
    l = 0xada9ad000ebb; break;
  case  971:
    l = 0xad9af5000eb8; break;
  case  972:
    l = 0xad8c3f000eb6; break;
  case  973:
    l = 0xad7d8b000eb4; break;
  case  974:
    l = 0xad6eda000eb1; break;
  case  975:
    l = 0xad602b000eaf; break;
  case  976:
    l = 0xad517f000eac; break;
  case  977:
    l = 0xad42d5000eaa; break;
  case  978:
    l = 0xad342e000ea7; break;
  case  979:
    l = 0xad2589000ea5; break;
  case  980:
    l = 0xad16e7000ea2; break;
  case  981:
    l = 0xad0847000ea0; break;
  case  982:
    l = 0xacf9aa000e9d; break;
  case  983:
    l = 0xaceb0f000e9b; break;
  case  984:
    l = 0xacdc77000e98; break;
  case  985:
    l = 0xaccde1000e96; break;
  case  986:
    l = 0xacbf4e000e93; break;
  case  987:
    l = 0xacb0bd000e91; break;
  case  988:
    l = 0xaca22e000e8f; break;
  case  989:
    l = 0xac93a2000e8c; break;
  case  990:
    l = 0xac8519000e89; break;
  case  991:
    l = 0xac7691000e88; break;
  case  992:
    l = 0xac680d000e84; break;
  case  993:
    l = 0xac598a000e83; break;
  case  994:
    l = 0xac4b0b000e7f; break;
  case  995:
    l = 0xac3c8d000e7e; break;
  case  996:
    l = 0xac2e12000e7b; break;
  case  997:
    l = 0xac1f9a000e78; break;
  case  998:
    l = 0xac1124000e76; break;
  case  999:
    l = 0xac02b0000e74; break;
  case 1000:
    l = 0xabf43f000e71; break;
  case 1001:
    l = 0xabe5d0000e6f; break;
  case 1002:
    l = 0xabd763000e6d; break;
  case 1003:
    l = 0xabc8f9000e6a; break;
  case 1004:
    l = 0xabba92000e67; break;
  case 1005:
    l = 0xabac2d000e65; break;
  case 1006:
    l = 0xab9dca000e63; break;
  case 1007:
    l = 0xab8f6a000e60; break;
  case 1008:
    l = 0xab810c000e5e; break;
  case 1009:
    l = 0xab72b0000e5c; break;
  case 1010:
    l = 0xab6457000e59; break;
  case 1011:
    l = 0xab5601000e56; break;
  case 1012:
    l = 0xab47ac000e55; break;
  case 1013:
    l = 0xab395a000e52; break;
  case 1014:
    l = 0xab2b0b000e4f; break;
  case 1015:
    l = 0xab1cbe000e4d; break;
  case 1016:
    l = 0xab0e73000e4b; break;
  case 1017:
    l = 0xab002b000e48; break;
  case 1018:
    l = 0xaaf1e5000e46; break;
  case 1019:
    l = 0xaae3a1000e44; break;
  case 1020:
    l = 0xaad560000e41; break;
  case 1021:
    l = 0xaac721000e3f; break;
  case 1022:
    l = 0xaab8e5000e3c; break;
  case 1023:
    l = 0xaaaaab000e3a; break;
  case 1024:
    l = 0xaa9c73000e38; break;
  case 1025:
    l = 0xaa8e3d000e36; break;
  case 1026:
    l = 0xaa800b000e32; break;
  case 1027:
    l = 0xaa71da000e31; break;
  case 1028:
    l = 0xaa63ac000e2e; break;
  case 1029:
    l = 0xaa5580000e2c; break;
  case 1030:
    l = 0xaa4756000e2a; break;
  case 1031:
    l = 0xaa392f000e27; break;
  case 1032:
    l = 0xaa2b0a000e25; break;
  case 1033:
    l = 0xaa1ce8000e22; break;
  case 1034:
    l = 0xaa0ec8000e20; break;
  case 1035:
    l = 0xaa00aa000e1e; break;
  case 1036:
    l = 0xa9f28e000e1c; break;
  case 1037:
    l = 0xa9e475000e19; break;
  case 1038:
    l = 0xa9d65f000e16; break;
  case 1039:
    l = 0xa9c84a000e15; break;
  case 1040:
    l = 0xa9ba38000e12; break;
  case 1041:
    l = 0xa9ac28000e10; break;
  case 1042:
    l = 0xa99e1b000e0d; break;
  case 1043:
    l = 0xa99010000e0b; break;
  case 1044:
    l = 0xa98207000e09; break;
  case 1045:
    l = 0xa97401000e06; break;
  case 1046:
    l = 0xa965fc000e05; break;
  case 1047:
    l = 0xa957fb000e01; break;
  case 1048:
    l = 0xa949fb000e00; break;
  case 1049:
    l = 0xa93bfe000dfd; break;
  case 1050:
    l = 0xa92e03000dfb; break;
  case 1051:
    l = 0xa9200a000df9; break;
  case 1052:
    l = 0xa91214000df6; break;
  case 1053:
    l = 0xa90420000df4; break;
  case 1054:
    l = 0xa8f62f000df1; break;
  case 1055:
    l = 0xa8e83f000df0; break;
  case 1056:
    l = 0xa8da52000ded; break;
  case 1057:
    l = 0xa8cc67000deb; break;
  case 1058:
    l = 0xa8be7f000de8; break;
  case 1059:
    l = 0xa8b099000de6; break;
  case 1060:
    l = 0xa8a2b5000de4; break;
  case 1061:
    l = 0xa894d3000de2; break;
  case 1062:
    l = 0xa886f4000ddf; break;
  case 1063:
    l = 0xa87917000ddd; break;
  case 1064:
    l = 0xa86b3c000ddb; break;
  case 1065:
    l = 0xa85d64000dd8; break;
  case 1066:
    l = 0xa84f8d000dd7; break;
  case 1067:
    l = 0xa841ba000dd3; break;
  case 1068:
    l = 0xa833e8000dd2; break;
  case 1069:
    l = 0xa82618000dd0; break;
  case 1070:
    l = 0xa8184b000dcd; break;
  case 1071:
    l = 0xa80a81000dca; break;
  case 1072:
    l = 0xa7fcb8000dc9; break;
  case 1073:
    l = 0xa7eef2000dc6; break;
  case 1074:
    l = 0xa7e12e000dc4; break;
  case 1075:
    l = 0xa7d36c000dc2; break;
  case 1076:
    l = 0xa7c5ac000dc0; break;
  case 1077:
    l = 0xa7b7ef000dbd; break;
  case 1078:
    l = 0xa7aa34000dbb; break;
  case 1079:
    l = 0xa79c7b000db9; break;
  case 1080:
    l = 0xa78ec4000db7; break;
  case 1081:
    l = 0xa78110000db4; break;
  case 1082:
    l = 0xa7735e000db2; break;
  case 1083:
    l = 0xa765ae000db0; break;
  case 1084:
    l = 0xa75801000dad; break;
  case 1085:
    l = 0xa74a55000dac; break;
  case 1086:
    l = 0xa73cac000da9; break;
  case 1087:
    l = 0xa72f05000da7; break;
  case 1088:
    l = 0xa72160000da5; break;
  case 1089:
    l = 0xa713be000da2; break;
  case 1090:
    l = 0xa7061e000da0; break;
  case 1091:
    l = 0xa6f880000d9e; break;
  case 1092:
    l = 0xa6eae4000d9c; break;
  case 1093:
    l = 0xa6dd4a000d9a; break;
  case 1094:
    l = 0xa6cfb3000d97; break;
  case 1095:
    l = 0xa6c21e000d95; break;
  case 1096:
    l = 0xa6b48b000d93; break;
  case 1097:
    l = 0xa6a6fa000d91; break;
  case 1098:
    l = 0xa6996c000d8e; break;
  case 1099:
    l = 0xa68bdf000d8d; break;
  case 1100:
    l = 0xa67e55000d8a; break;
  case 1101:
    l = 0xa670cd000d88; break;
  case 1102:
    l = 0xa66348000d85; break;
  case 1103:
    l = 0xa655c4000d84; break;
  case 1104:
    l = 0xa64843000d81; break;
  case 1105:
    l = 0xa63ac4000d7f; break;
  case 1106:
    l = 0xa62d47000d7d; break;
  case 1107:
    l = 0xa61fcc000d7b; break;
  case 1108:
    l = 0xa61253000d79; break;
  case 1109:
    l = 0xa604dd000d76; break;
  case 1110:
    l = 0xa5f769000d74; break;
  case 1111:
    l = 0xa5e9f7000d72; break;
  case 1112:
    l = 0xa5dc87000d70; break;
  case 1113:
    l = 0xa5cf19000d6e; break;
  case 1114:
    l = 0xa5c1ae000d6b; break;
  case 1115:
    l = 0xa5b444000d6a; break;
  case 1116:
    l = 0xa5a6dd000d67; break;
  case 1117:
    l = 0xa59978000d65; break;
  case 1118:
    l = 0xa58c16000d62; break;
  case 1119:
    l = 0xa57eb5000d61; break;
  case 1120:
    l = 0xa57156000d5f; break;
  case 1121:
    l = 0xa563fa000d5c; break;
  case 1122:
    l = 0xa556a0000d5a; break;
  case 1123:
    l = 0xa54948000d58; break;
  case 1124:
    l = 0xa53bf2000d56; break;
  case 1125:
    l = 0xa52e9e000d54; break;
  case 1126:
    l = 0xa5214d000d51; break;
  case 1127:
    l = 0xa513fd000d50; break;
  case 1128:
    l = 0xa506b0000d4d; break;
  case 1129:
    l = 0xa4f965000d4b; break;
  case 1130:
    l = 0xa4ec1c000d49; break;
  case 1131:
    l = 0xa4ded5000d47; break;
  case 1132:
    l = 0xa4d190000d45; break;
  case 1133:
    l = 0xa4c44e000d42; break;
  case 1134:
    l = 0xa4b70d000d41; break;
  case 1135:
    l = 0xa4a9cf000d3e; break;
  case 1136:
    l = 0xa49c93000d3c; break;
  case 1137:
    l = 0xa48f59000d3a; break;
  case 1138:
    l = 0xa48221000d38; break;
  case 1139:
    l = 0xa474eb000d36; break;
  case 1140:
    l = 0xa467b7000d34; break;
  case 1141:
    l = 0xa45a86000d31; break;
  case 1142:
    l = 0xa44d56000d30; break;
  case 1143:
    l = 0xa44029000d2d; break;
  case 1144:
    l = 0xa432fe000d2b; break;
  case 1145:
    l = 0xa425d5000d29; break;
  case 1146:
    l = 0xa418ae000d27; break;
  case 1147:
    l = 0xa40b89000d25; break;
  case 1148:
    l = 0xa3fe66000d23; break;
  case 1149:
    l = 0xa3f145000d21; break;
  case 1150:
    l = 0xa3e427000d1e; break;
  case 1151:
    l = 0xa3d70a000d1d; break;
  case 1152:
    l = 0xa3c9f0000d1a; break;
  case 1153:
    l = 0xa3bcd7000d19; break;
  case 1154:
    l = 0xa3afc1000d16; break;
  case 1155:
    l = 0xa3a2ad000d14; break;
  case 1156:
    l = 0xa3959b000d12; break;
  case 1157:
    l = 0xa3888b000d10; break;
  case 1158:
    l = 0xa37b7d000d0e; break;
  case 1159:
    l = 0xa36e72000d0b; break;
  case 1160:
    l = 0xa36168000d0a; break;
  case 1161:
    l = 0xa35460000d08; break;
  case 1162:
    l = 0xa3475b000d05; break;
  case 1163:
    l = 0xa33a57000d04; break;
  case 1164:
    l = 0xa32d56000d01; break;
  case 1165:
    l = 0xa32057000cff; break;
  case 1166:
    l = 0xa31359000cfe; break;
  case 1167:
    l = 0xa3065e000cfb; break;
  case 1168:
    l = 0xa2f965000cf9; break;
  case 1169:
    l = 0xa2ec6e000cf7; break;
  case 1170:
    l = 0xa2df79000cf5; break;
  case 1171:
    l = 0xa2d286000cf3; break;
  case 1172:
    l = 0xa2c595000cf1; break;
  case 1173:
    l = 0xa2b8a6000cef; break;
  case 1174:
    l = 0xa2abba000cec; break;
  case 1175:
    l = 0xa29ecf000ceb; break;
  case 1176:
    l = 0xa291e6000ce9; break;
  case 1177:
    l = 0xa28500000ce6; break;
  case 1178:
    l = 0xa2781b000ce5; break;
  case 1179:
    l = 0xa26b39000ce2; break;
  case 1180:
    l = 0xa25e58000ce1; break;
  case 1181:
    l = 0xa2517a000cde; break;
  case 1182:
    l = 0xa2449d000cdd; break;
  case 1183:
    l = 0xa237c3000cda; break;
  case 1184:
    l = 0xa22aeb000cd8; break;
  case 1185:
    l = 0xa21e14000cd7; break;
  case 1186:
    l = 0xa21140000cd4; break;
  case 1187:
    l = 0xa2046e000cd2; break;
  case 1188:
    l = 0xa1f79e000cd0; break;
  case 1189:
    l = 0xa1ead0000cce; break;
  case 1190:
    l = 0xa1de04000ccc; break;
  case 1191:
    l = 0xa1d139000ccb; break;
  case 1192:
    l = 0xa1c471000cc8; break;
  case 1193:
    l = 0xa1b7ab000cc6; break;
  case 1194:
    l = 0xa1aae7000cc4; break;
  case 1195:
    l = 0xa19e25000cc2; break;
  case 1196:
    l = 0xa19165000cc0; break;
  case 1197:
    l = 0xa184a7000cbe; break;
  case 1198:
    l = 0xa177eb000cbc; break;
  case 1199:
    l = 0xa16b31000cba; break;
  case 1200:
    l = 0xa15e79000cb8; break;
  case 1201:
    l = 0xa151c3000cb6; break;
  case 1202:
    l = 0xa1450f000cb4; break;
  case 1203:
    l = 0xa1385d000cb2; break;
  case 1204:
    l = 0xa12bad000cb0; break;
  case 1205:
    l = 0xa11eff000cae; break;
  case 1206:
    l = 0xa11253000cac; break;
  case 1207:
    l = 0xa105a9000caa; break;
  case 1208:
    l = 0xa0f901000ca8; break;
  case 1209:
    l = 0xa0ec5b000ca6; break;
  case 1210:
    l = 0xa0dfb7000ca4; break;
  case 1211:
    l = 0xa0d315000ca2; break;
  case 1212:
    l = 0xa0c675000ca0; break;
  case 1213:
    l = 0xa0b9d7000c9e; break;
  case 1214:
    l = 0xa0ad3b000c9c; break;
  case 1215:
    l = 0xa0a0a1000c9a; break;
  case 1216:
    l = 0xa09408000c99; break;
  case 1217:
    l = 0xa08772000c96; break;
  case 1218:
    l = 0xa07ade000c94; break;
  case 1219:
    l = 0xa06e4c000c92; break;
  case 1220:
    l = 0xa061bb000c91; break;
  case 1221:
    l = 0xa0552d000c8e; break;
  case 1222:
    l = 0xa048a1000c8c; break;
  case 1223:
    l = 0xa03c16000c8b; break;
  case 1224:
    l = 0xa02f8e000c88; break;
  case 1225:
    l = 0xa02308000c86; break;
  case 1226:
    l = 0xa01683000c85; break;
  case 1227:
    l = 0xa00a01000c82; break;
  case 1228:
    l = 0x9ffd80000c81; break;
  case 1229:
    l = 0x9ff101000c7f; break;
  case 1230:
    l = 0x9fe485000c7c; break;
  case 1231:
    l = 0x9fd80a000c7b; break;
  case 1232:
    l = 0x9fcb91000c79; break;
  case 1233:
    l = 0x9fbf1a000c77; break;
  case 1234:
    l = 0x9fb2a5000c75; break;
  case 1235:
    l = 0x9fa632000c73; break;
  case 1236:
    l = 0x9f99c1000c71; break;
  case 1237:
    l = 0x9f8d52000c6f; break;
  case 1238:
    l = 0x9f80e5000c6d; break;
  case 1239:
    l = 0x9f747a000c6b; break;
  case 1240:
    l = 0x9f6811000c69; break;
  case 1241:
    l = 0x9f5ba9000c68; break;
  case 1242:
    l = 0x9f4f44000c65; break;
  case 1243:
    l = 0x9f42e0000c64; break;
  case 1244:
    l = 0x9f367f000c61; break;
  case 1245:
    l = 0x9f2a1f000c60; break;
  case 1246:
    l = 0x9f1dc2000c5d; break;
  case 1247:
    l = 0x9f1166000c5c; break;
  case 1248:
    l = 0x9f050c000c5a; break;
  case 1249:
    l = 0x9ef8b4000c58; break;
  case 1250:
    l = 0x9eec5e000c56; break;
  case 1251:
    l = 0x9ee00a000c54; break;
  case 1252:
    l = 0x9ed3b8000c52; break;
  case 1253:
    l = 0x9ec767000c51; break;
  case 1254:
    l = 0x9ebb19000c4e; break;
  case 1255:
    l = 0x9eaecc000c4d; break;
  case 1256:
    l = 0x9ea282000c4a; break;
  case 1257:
    l = 0x9e9639000c49; break;
  case 1258:
    l = 0x9e89f2000c47; break;
  case 1259:
    l = 0x9e7dae000c44; break;
  case 1260:
    l = 0x9e716b000c43; break;
  case 1261:
    l = 0x9e6529000c42; break;
  case 1262:
    l = 0x9e58ea000c3f; break;
  case 1263:
    l = 0x9e4cad000c3d; break;
  case 1264:
    l = 0x9e4072000c3b; break;
  case 1265:
    l = 0x9e3438000c3a; break;
  case 1266:
    l = 0x9e2800000c38; break;
  case 1267:
    l = 0x9e1bcb000c35; break;
  case 1268:
    l = 0x9e0f97000c34; break;
  case 1269:
    l = 0x9e0365000c32; break;
  case 1270:
    l = 0x9df735000c30; break;
  case 1271:
    l = 0x9deb07000c2e; break;
  case 1272:
    l = 0x9ddeda000c2d; break;
  case 1273:
    l = 0x9dd2b0000c2a; break;
  case 1274:
    l = 0x9dc687000c29; break;
  case 1275:
    l = 0x9dba61000c26; break;
  case 1276:
    l = 0x9dae3c000c25; break;
  case 1277:
    l = 0x9da219000c23; break;
  case 1278:
    l = 0x9d95f8000c21; break;
  case 1279:
    l = 0x9d89d8000c20; break;
  case 1280:
    l = 0x9d7dbb000c1d; break;
  case 1281:
    l = 0x9d71a0000c1b; break;
  case 1282:
    l = 0x9d6586000c1a; break;
  case 1283:
    l = 0x9d596e000c18; break;
  case 1284:
    l = 0x9d4d58000c16; break;
  case 1285:
    l = 0x9d4144000c14; break;
  case 1286:
    l = 0x9d3532000c12; break;
  case 1287:
    l = 0x9d2922000c10; break;
  case 1288:
    l = 0x9d1d13000c0f; break;
  case 1289:
    l = 0x9d1106000c0d; break;
  case 1290:
    l = 0x9d04fc000c0a; break;
  case 1291:
    l = 0x9cf8f3000c09; break;
  case 1292:
    l = 0x9cecec000c07; break;
  case 1293:
    l = 0x9ce0e6000c06; break;
  case 1294:
    l = 0x9cd4e3000c03; break;
  case 1295:
    l = 0x9cc8e1000c02; break;
  case 1296:
    l = 0x9cbce2000bff; break;
  case 1297:
    l = 0x9cb0e4000bfe; break;
  case 1298:
    l = 0x9ca4e7000bfd; break;
  case 1299:
    l = 0x9c98ed000bfa; break;
  case 1300:
    l = 0x9c8cf5000bf8; break;
  case 1301:
    l = 0x9c80fe000bf7; break;
  case 1302:
    l = 0x9c7509000bf5; break;
  case 1303:
    l = 0x9c6916000bf3; break;
  case 1304:
    l = 0x9c5d25000bf1; break;
  case 1305:
    l = 0x9c5136000bef; break;
  case 1306:
    l = 0x9c4549000bed; break;
  case 1307:
    l = 0x9c395d000bec; break;
  case 1308:
    l = 0x9c2d73000bea; break;
  case 1309:
    l = 0x9c218b000be8; break;
  case 1310:
    l = 0x9c15a5000be6; break;
  case 1311:
    l = 0x9c09c0000be5; break;
  case 1312:
    l = 0x9bfdde000be2; break;
  case 1313:
    l = 0x9bf1fd000be1; break;
  case 1314:
    l = 0x9be61e000bdf; break;
  case 1315:
    l = 0x9bda41000bdd; break;
  case 1316:
    l = 0x9bce66000bdb; break;
  case 1317:
    l = 0x9bc28c000bda; break;
  case 1318:
    l = 0x9bb6b4000bd8; break;
  case 1319:
    l = 0x9baade000bd6; break;
  case 1320:
    l = 0x9b9f0a000bd4; break;
  case 1321:
    l = 0x9b9338000bd2; break;
  case 1322:
    l = 0x9b8767000bd1; break;
  case 1323:
    l = 0x9b7b99000bce; break;
  case 1324:
    l = 0x9b6fcc000bcd; break;
  case 1325:
    l = 0x9b6400000bcc; break;
  case 1326:
    l = 0x9b5837000bc9; break;
  case 1327:
    l = 0x9b4c70000bc7; break;
  case 1328:
    l = 0x9b40aa000bc6; break;
  case 1329:
    l = 0x9b34e6000bc4; break;
  case 1330:
    l = 0x9b2923000bc3; break;
  case 1331:
    l = 0x9b1d63000bc0; break;
  case 1332:
    l = 0x9b11a4000bbf; break;
  case 1333:
    l = 0x9b05e7000bbd; break;
  case 1334:
    l = 0x9afa2c000bbb; break;
  case 1335:
    l = 0x9aee73000bb9; break;
  case 1336:
    l = 0x9ae2bb000bb8; break;
  case 1337:
    l = 0x9ad705000bb6; break;
  case 1338:
    l = 0x9acb51000bb4; break;
  case 1339:
    l = 0x9abf9f000bb2; break;
  case 1340:
    l = 0x9ab3ef000bb0; break;
  case 1341:
    l = 0x9aa840000baf; break;
  case 1342:
    l = 0x9a9c93000bad; break;
  case 1343:
    l = 0x9a90e8000bab; break;
  case 1344:
    l = 0x9a853e000baa; break;
  case 1345:
    l = 0x9a7997000ba7; break;
  case 1346:
    l = 0x9a6df1000ba6; break;
  case 1347:
    l = 0x9a624c000ba5; break;
  case 1348:
    l = 0x9a56aa000ba2; break;
  case 1349:
    l = 0x9a4b09000ba1; break;
  case 1350:
    l = 0x9a3f6a000b9f; break;
  case 1351:
    l = 0x9a33cd000b9d; break;
  case 1352:
    l = 0x9a2832000b9b; break;
  case 1353:
    l = 0x9a1c98000b9a; break;
  case 1354:
    l = 0x9a1100000b98; break;
  case 1355:
    l = 0x9a056a000b96; break;
  case 1356:
    l = 0x99f9d6000b94; break;
  case 1357:
    l = 0x99ee43000b93; break;
  case 1358:
    l = 0x99e2b2000b91; break;
  case 1359:
    l = 0x99d723000b8f; break;
  case 1360:
    l = 0x99cb95000b8e; break;
  case 1361:
    l = 0x99c00a000b8b; break;
  case 1362:
    l = 0x99b47f000b8b; break;
  case 1363:
    l = 0x99a8f7000b88; break;
  case 1364:
    l = 0x999d71000b86; break;
  case 1365:
    l = 0x9991ec000b85; break;
  case 1366:
    l = 0x998669000b83; break;
  case 1367:
    l = 0x997ae7000b82; break;
  case 1368:
    l = 0x996f68000b7f; break;
  case 1369:
    l = 0x9963ea000b7e; break;
  case 1370:
    l = 0x99586e000b7c; break;
  case 1371:
    l = 0x994cf3000b7b; break;
  case 1372:
    l = 0x99417a000b79; break;
  case 1373:
    l = 0x993603000b77; break;
  case 1374:
    l = 0x992a8e000b75; break;
  case 1375:
    l = 0x991f1a000b74; break;
  case 1376:
    l = 0x9913a8000b72; break;
  case 1377:
    l = 0x990838000b70; break;
  case 1378:
    l = 0x98fcca000b6e; break;
  case 1379:
    l = 0x98f15d000b6d; break;
  case 1380:
    l = 0x98e5f2000b6b; break;
  case 1381:
    l = 0x98da88000b6a; break;
  case 1382:
    l = 0x98cf21000b67; break;
  case 1383:
    l = 0x98c3bb000b66; break;
  case 1384:
    l = 0x98b856000b65; break;
  case 1385:
    l = 0x98acf4000b62; break;
  case 1386:
    l = 0x98a193000b61; break;
  case 1387:
    l = 0x989634000b5f; break;
  case 1388:
    l = 0x988ad6000b5e; break;
  case 1389:
    l = 0x987f7a000b5c; break;
  case 1390:
    l = 0x987420000b5a; break;
  case 1391:
    l = 0x9868c8000b58; break;
  case 1392:
    l = 0x985d71000b57; break;
  case 1393:
    l = 0x98521c000b55; break;
  case 1394:
    l = 0x9846c9000b53; break;
  case 1395:
    l = 0x983b77000b52; break;
  case 1396:
    l = 0x983027000b50; break;
  case 1397:
    l = 0x9824d9000b4e; break;
  case 1398:
    l = 0x98198c000b4d; break;
  case 1399:
    l = 0x980e41000b4b; break;
  case 1400:
    l = 0x9802f8000b49; break;
  case 1401:
    l = 0x97f7b0000b48; break;
  case 1402:
    l = 0x97ec6a000b46; break;
  case 1403:
    l = 0x97e126000b44; break;
  case 1404:
    l = 0x97d5e4000b42; break;
  case 1405:
    l = 0x97caa3000b41; break;
  case 1406:
    l = 0x97bf63000b40; break;
  case 1407:
    l = 0x97b426000b3d; break;
  case 1408:
    l = 0x97a8ea000b3c; break;
  case 1409:
    l = 0x979db0000b3a; break;
  case 1410:
    l = 0x979277000b39; break;
  case 1411:
    l = 0x978740000b37; break;
  case 1412:
    l = 0x977c0b000b35; break;
  case 1413:
    l = 0x9770d7000b34; break;
  case 1414:
    l = 0x9765a5000b32; break;
  case 1415:
    l = 0x975a75000b30; break;
  case 1416:
    l = 0x974f46000b2f; break;
  case 1417:
    l = 0x974419000b2d; break;
  case 1418:
    l = 0x9738ee000b2b; break;
  case 1419:
    l = 0x972dc4000b2a; break;
  case 1420:
    l = 0x97229c000b28; break;
  case 1421:
    l = 0x971776000b26; break;
  case 1422:
    l = 0x970c51000b25; break;
  case 1423:
    l = 0x97012e000b23; break;
  case 1424:
    l = 0x96f60c000b22; break;
  case 1425:
    l = 0x96eaed000b1f; break;
  case 1426:
    l = 0x96dfce000b1f; break;
  case 1427:
    l = 0x96d4b2000b1c; break;
  case 1428:
    l = 0x96c997000b1b; break;
  case 1429:
    l = 0x96be7e000b19; break;
  case 1430:
    l = 0x96b366000b18; break;
  case 1431:
    l = 0x96a850000b16; break;
  case 1432:
    l = 0x969d3c000b14; break;
  case 1433:
    l = 0x969229000b13; break;
  case 1434:
    l = 0x968718000b11; break;
  case 1435:
    l = 0x967c08000b10; break;
  case 1436:
    l = 0x9670fa000b0e; break;
  case 1437:
    l = 0x9665ee000b0c; break;
  case 1438:
    l = 0x965ae3000b0b; break;
  case 1439:
    l = 0x964fda000b09; break;
  case 1440:
    l = 0x9644d3000b07; break;
  case 1441:
    l = 0x9639cd000b06; break;
  case 1442:
    l = 0x962ec9000b04; break;
  case 1443:
    l = 0x9623c6000b03; break;
  case 1444:
    l = 0x9618c5000b01; break;
  case 1445:
    l = 0x960dc6000aff; break;
  case 1446:
    l = 0x9602c8000afe; break;
  case 1447:
    l = 0x95f7cc000afc; break;
  case 1448:
    l = 0x95ecd2000afa; break;
  case 1449:
    l = 0x95e1d9000af9; break;
  case 1450:
    l = 0x95d6e2000af7; break;
  case 1451:
    l = 0x95cbec000af6; break;
  case 1452:
    l = 0x95c0f8000af4; break;
  case 1453:
    l = 0x95b605000af3; break;
  case 1454:
    l = 0x95ab15000af0; break;
  case 1455:
    l = 0x95a025000af0; break;
  case 1456:
    l = 0x959538000aed; break;
  case 1457:
    l = 0x958a4c000aec; break;
  case 1458:
    l = 0x957f61000aeb; break;
  case 1459:
    l = 0x957478000ae9; break;
  case 1460:
    l = 0x956991000ae7; break;
  case 1461:
    l = 0x955eab000ae6; break;
  case 1462:
    l = 0x9553c7000ae4; break;
  case 1463:
    l = 0x9548e4000ae3; break;
  case 1464:
    l = 0x953e04000ae0; break;
  case 1465:
    l = 0x953324000ae0; break;
  case 1466:
    l = 0x952846000ade; break;
  case 1467:
    l = 0x951d6a000adc; break;
  case 1468:
    l = 0x951290000ada; break;
  case 1469:
    l = 0x9507b7000ad9; break;
  case 1470:
    l = 0x94fcdf000ad8; break;
  case 1471:
    l = 0x94f209000ad6; break;
  case 1472:
    l = 0x94e735000ad4; break;
  case 1473:
    l = 0x94dc62000ad3; break;
  case 1474:
    l = 0x94d191000ad1; break;
  case 1475:
    l = 0x94c6c1000ad0; break;
  case 1476:
    l = 0x94bbf3000ace; break;
  case 1477:
    l = 0x94b127000acc; break;
  case 1478:
    l = 0x94a65c000acb; break;
  case 1479:
    l = 0x949b93000ac9; break;
  case 1480:
    l = 0x9490cb000ac8; break;
  case 1481:
    l = 0x948605000ac6; break;
  case 1482:
    l = 0x947b40000ac5; break;
  case 1483:
    l = 0x94707d000ac3; break;
  case 1484:
    l = 0x9465bc000ac1; break;
  case 1485:
    l = 0x945afc000ac0; break;
  case 1486:
    l = 0x94503d000abf; break;
  case 1487:
    l = 0x944580000abd; break;
  case 1488:
    l = 0x943ac5000abb; break;
  case 1489:
    l = 0x94300b000aba; break;
  case 1490:
    l = 0x942553000ab8; break;
  case 1491:
    l = 0x941a9d000ab6; break;
  case 1492:
    l = 0x940fe8000ab5; break;
  case 1493:
    l = 0x940534000ab4; break;
  case 1494:
    l = 0x93fa82000ab2; break;
  case 1495:
    l = 0x93efd2000ab0; break;
  case 1496:
    l = 0x93e523000aaf; break;
  case 1497:
    l = 0x93da75000aae; break;
  case 1498:
    l = 0x93cfca000aab; break;
  case 1499:
    l = 0x93c51f000aab; break;
  case 1500:
    l = 0x93ba77000aa8; break;
  case 1501:
    l = 0x93afcf000aa8; break;
  case 1502:
    l = 0x93a52a000aa5; break;
  case 1503:
    l = 0x939a86000aa4; break;
  case 1504:
    l = 0x938fe3000aa3; break;
  case 1505:
    l = 0x938542000aa1; break;
  case 1506:
    l = 0x937aa3000a9f; break;
  case 1507:
    l = 0x937005000a9e; break;
  case 1508:
    l = 0x936568000a9d; break;
  case 1509:
    l = 0x935acd000a9b; break;
  case 1510:
    l = 0x935034000a99; break;
  case 1511:
    l = 0x93459c000a98; break;
  case 1512:
    l = 0x933b05000a97; break;
  case 1513:
    l = 0x933071000a94; break;
  case 1514:
    l = 0x9325dd000a94; break;
  case 1515:
    l = 0x931b4b000a92; break;
  case 1516:
    l = 0x9310bb000a90; break;
  case 1517:
    l = 0x93062c000a8f; break;
  case 1518:
    l = 0x92fb9f000a8d; break;
  case 1519:
    l = 0x92f113000a8c; break;
  case 1520:
    l = 0x92e689000a8a; break;
  case 1521:
    l = 0x92dc00000a89; break;
  case 1522:
    l = 0x92d179000a87; break;
  case 1523:
    l = 0x92c6f4000a85; break;
  case 1524:
    l = 0x92bc6f000a85; break;
  case 1525:
    l = 0x92b1ed000a82; break;
  case 1526:
    l = 0x92a76c000a81; break;
  case 1527:
    l = 0x929cec000a80; break;
  case 1528:
    l = 0x92926e000a7e; break;
  case 1529:
    l = 0x9287f1000a7d; break;
  case 1530:
    l = 0x927d76000a7b; break;
  case 1531:
    l = 0x9272fc000a7a; break;
  case 1532:
    l = 0x926884000a78; break;
  case 1533:
    l = 0x925e0d000a77; break;
  case 1534:
    l = 0x925398000a75; break;
  case 1535:
    l = 0x924924000a74; break;
  case 1536:
    l = 0x923eb2000a72; break;
  case 1537:
    l = 0x923442000a70; break;
  case 1538:
    l = 0x9229d2000a70; break;
  case 1539:
    l = 0x921f65000a6d; break;
  case 1540:
    l = 0x9214f8000a6d; break;
  case 1541:
    l = 0x920a8e000a6a; break;
  case 1542:
    l = 0x920024000a6a; break;
  case 1543:
    l = 0x91f5bd000a67; break;
  case 1544:
    l = 0x91eb56000a67; break;
  case 1545:
    l = 0x91e0f2000a64; break;
  case 1546:
    l = 0x91d68e000a64; break;
  case 1547:
    l = 0x91cc2c000a62; break;
  case 1548:
    l = 0x91c1cc000a60; break;
  case 1549:
    l = 0x91b76d000a5f; break;
  case 1550:
    l = 0x91ad10000a5d; break;
  case 1551:
    l = 0x91a2b4000a5c; break;
  case 1552:
    l = 0x919859000a5b; break;
  case 1553:
    l = 0x918e00000a59; break;
  case 1554:
    l = 0x9183a9000a57; break;
  case 1555:
    l = 0x917953000a56; break;
  case 1556:
    l = 0x916efe000a55; break;
  case 1557:
    l = 0x9164ab000a53; break;
  case 1558:
    l = 0x915a59000a52; break;
  case 1559:
    l = 0x915009000a50; break;
  case 1560:
    l = 0x9145ba000a4f; break;
  case 1561:
    l = 0x913b6d000a4d; break;
  case 1562:
    l = 0x913121000a4c; break;
  case 1563:
    l = 0x9126d7000a4a; break;
  case 1564:
    l = 0x911c8e000a49; break;
  case 1565:
    l = 0x911246000a48; break;
  case 1566:
    l = 0x910800000a46; break;
  case 1567:
    l = 0x90fdbc000a44; break;
  case 1568:
    l = 0x90f379000a43; break;
  case 1569:
    l = 0x90e937000a42; break;
  case 1570:
    l = 0x90def7000a40; break;
  case 1571:
    l = 0x90d4b8000a3f; break;
  case 1572:
    l = 0x90ca7b000a3d; break;
  case 1573:
    l = 0x90c03f000a3c; break;
  case 1574:
    l = 0x90b605000a3a; break;
  case 1575:
    l = 0x90abcc000a39; break;
  case 1576:
    l = 0x90a194000a38; break;
  case 1577:
    l = 0x90975e000a36; break;
  case 1578:
    l = 0x908d2a000a34; break;
  case 1579:
    l = 0x9082f7000a33; break;
  case 1580:
    l = 0x9078c5000a32; break;
  case 1581:
    l = 0x906e95000a30; break;
  case 1582:
    l = 0x906466000a2f; break;
  case 1583:
    l = 0x905a38000a2e; break;
  case 1584:
    l = 0x90500c000a2c; break;
  case 1585:
    l = 0x9045e2000a2a; break;
  case 1586:
    l = 0x903bb9000a29; break;
  case 1587:
    l = 0x903191000a28; break;
  case 1588:
    l = 0x90276b000a26; break;
  case 1589:
    l = 0x901d46000a25; break;
  case 1590:
    l = 0x901322000a24; break;
  case 1591:
    l = 0x900900000a22; break;
  case 1592:
    l = 0x8ffee0000a20; break;
  case 1593:
    l = 0x8ff4c1000a1f; break;
  case 1594:
    l = 0x8feaa3000a1e; break;
  case 1595:
    l = 0x8fe087000a1c; break;
  case 1596:
    l = 0x8fd66c000a1b; break;
  case 1597:
    l = 0x8fcc52000a1a; break;
  case 1598:
    l = 0x8fc23a000a18; break;
  case 1599:
    l = 0x8fb824000a16; break;
  case 1600:
    l = 0x8fae0f000a15; break;
  case 1601:
    l = 0x8fa3fb000a14; break;
  case 1602:
    l = 0x8f99e8000a13; break;
  case 1603:
    l = 0x8f8fd8000a10; break;
  case 1604:
    l = 0x8f85c8000a10; break;
  case 1605:
    l = 0x8f7bba000a0e; break;
  case 1606:
    l = 0x8f71ad000a0d; break;
  case 1607:
    l = 0x8f67a2000a0b; break;
  case 1608:
    l = 0x8f5d98000a0a; break;
  case 1609:
    l = 0x8f538f000a09; break;
  case 1610:
    l = 0x8f4988000a07; break;
  case 1611:
    l = 0x8f3f83000a05; break;
  case 1612:
    l = 0x8f357e000a05; break;
  case 1613:
    l = 0x8f2b7b000a03; break;
  case 1614:
    l = 0x8f217a000a01; break;
  case 1615:
    l = 0x8f177a000a00; break;
  case 1616:
    l = 0x8f0d7b0009ff; break;
  case 1617:
    l = 0x8f037e0009fd; break;
  case 1618:
    l = 0x8ef9820009fc; break;
  case 1619:
    l = 0x8eef870009fb; break;
  case 1620:
    l = 0x8ee58e0009f9; break;
  case 1621:
    l = 0x8edb960009f8; break;
  case 1622:
    l = 0x8ed1a00009f6; break;
  case 1623:
    l = 0x8ec7ab0009f5; break;
  case 1624:
    l = 0x8ebdb80009f3; break;
  case 1625:
    l = 0x8eb3c50009f3; break;
  case 1626:
    l = 0x8ea9d50009f0; break;
  case 1627:
    l = 0x8e9fe50009f0; break;
  case 1628:
    l = 0x8e95f70009ee; break;
  case 1629:
    l = 0x8e8c0a0009ed; break;
  case 1630:
    l = 0x8e821f0009eb; break;
  case 1631:
    l = 0x8e78350009ea; break;
  case 1632:
    l = 0x8e6e4d0009e8; break;
  case 1633:
    l = 0x8e64660009e7; break;
  case 1634:
    l = 0x8e5a800009e6; break;
  case 1635:
    l = 0x8e509c0009e4; break;
  case 1636:
    l = 0x8e46b90009e3; break;
  case 1637:
    l = 0x8e3cd70009e2; break;
  case 1638:
    l = 0x8e32f70009e0; break;
  case 1639:
    l = 0x8e29180009df; break;
  case 1640:
    l = 0x8e1f3a0009de; break;
  case 1641:
    l = 0x8e155e0009dc; break;
  case 1642:
    l = 0x8e0b830009db; break;
  case 1643:
    l = 0x8e01aa0009d9; break;
  case 1644:
    l = 0x8df7d20009d8; break;
  case 1645:
    l = 0x8dedfb0009d7; break;
  case 1646:
    l = 0x8de4260009d5; break;
  case 1647:
    l = 0x8dda520009d4; break;
  case 1648:
    l = 0x8dd07f0009d3; break;
  case 1649:
    l = 0x8dc6ae0009d1; break;
  case 1650:
    l = 0x8dbcde0009d0; break;
  case 1651:
    l = 0x8db3100009ce; break;
  case 1652:
    l = 0x8da9430009cd; break;
  case 1653:
    l = 0x8d9f770009cc; break;
  case 1654:
    l = 0x8d95ac0009cb; break;
  case 1655:
    l = 0x8d8be30009c9; break;
  case 1656:
    l = 0x8d821b0009c8; break;
  case 1657:
    l = 0x8d78550009c6; break;
  case 1658:
    l = 0x8d6e900009c5; break;
  case 1659:
    l = 0x8d64cc0009c4; break;
  case 1660:
    l = 0x8d5b0a0009c2; break;
  case 1661:
    l = 0x8d51490009c1; break;
  case 1662:
    l = 0x8d47890009c0; break;
  case 1663:
    l = 0x8d3dcb0009be; break;
  case 1664:
    l = 0x8d340e0009bd; break;
  case 1665:
    l = 0x8d2a520009bc; break;
  case 1666:
    l = 0x8d20980009ba; break;
  case 1667:
    l = 0x8d16df0009b9; break;
  case 1668:
    l = 0x8d0d280009b7; break;
  case 1669:
    l = 0x8d03710009b7; break;
  case 1670:
    l = 0x8cf9bc0009b5; break;
  case 1671:
    l = 0x8cf0090009b3; break;
  case 1672:
    l = 0x8ce6560009b3; break;
  case 1673:
    l = 0x8cdca60009b0; break;
  case 1674:
    l = 0x8cd2f60009b0; break;
  case 1675:
    l = 0x8cc9480009ae; break;
  case 1676:
    l = 0x8cbf9b0009ad; break;
  case 1677:
    l = 0x8cb5ef0009ac; break;
  case 1678:
    l = 0x8cac450009aa; break;
  case 1679:
    l = 0x8ca29c0009a9; break;
  case 1680:
    l = 0x8c98f40009a8; break;
  case 1681:
    l = 0x8c8f4e0009a6; break;
  case 1682:
    l = 0x8c85a90009a5; break;
  case 1683:
    l = 0x8c7c050009a4; break;
  case 1684:
    l = 0x8c72630009a2; break;
  case 1685:
    l = 0x8c68c20009a1; break;
  case 1686:
    l = 0x8c5f220009a0; break;
  case 1687:
    l = 0x8c558400099e; break;
  case 1688:
    l = 0x8c4be700099d; break;
  case 1689:
    l = 0x8c424b00099c; break;
  case 1690:
    l = 0x8c38b100099a; break;
  case 1691:
    l = 0x8c2f18000999; break;
  case 1692:
    l = 0x8c2580000998; break;
  case 1693:
    l = 0x8c1be9000997; break;
  case 1694:
    l = 0x8c1254000995; break;
  case 1695:
    l = 0x8c08c0000994; break;
  case 1696:
    l = 0x8bff2e000992; break;
  case 1697:
    l = 0x8bf59d000991; break;
  case 1698:
    l = 0x8bec0d000990; break;
  case 1699:
    l = 0x8be27e00098f; break;
  case 1700:
    l = 0x8bd8f100098d; break;
  case 1701:
    l = 0x8bcf6500098c; break;
  case 1702:
    l = 0x8bc5da00098b; break;
  case 1703:
    l = 0x8bbc51000989; break;
  case 1704:
    l = 0x8bb2c9000988; break;
  case 1705:
    l = 0x8ba942000987; break;
  case 1706:
    l = 0x8b9fbc000986; break;
  case 1707:
    l = 0x8b9638000984; break;
  case 1708:
    l = 0x8b8cb5000983; break;
  case 1709:
    l = 0x8b8334000981; break;
  case 1710:
    l = 0x8b79b3000981; break;
  case 1711:
    l = 0x8b703400097f; break;
  case 1712:
    l = 0x8b66b600097e; break;
  case 1713:
    l = 0x8b5d3a00097c; break;
  case 1714:
    l = 0x8b53bf00097b; break;
  case 1715:
    l = 0x8b4a4500097a; break;
  case 1716:
    l = 0x8b40cc000979; break;
  case 1717:
    l = 0x8b3755000977; break;
  case 1718:
    l = 0x8b2ddf000976; break;
  case 1719:
    l = 0x8b246a000975; break;
  case 1720:
    l = 0x8b1af7000973; break;
  case 1721:
    l = 0x8b1185000972; break;
  case 1722:
    l = 0x8b0814000971; break;
  case 1723:
    l = 0x8afea4000970; break;
  case 1724:
    l = 0x8af53600096e; break;
  case 1725:
    l = 0x8aebc900096d; break;
  case 1726:
    l = 0x8ae25d00096c; break;
  case 1727:
    l = 0x8ad8f300096a; break;
  case 1728:
    l = 0x8acf8a000969; break;
  case 1729:
    l = 0x8ac622000968; break;
  case 1730:
    l = 0x8abcbb000967; break;
  case 1731:
    l = 0x8ab356000965; break;
  case 1732:
    l = 0x8aa9f2000964; break;
  case 1733:
    l = 0x8aa08f000963; break;
  case 1734:
    l = 0x8a972d000962; break;
  case 1735:
    l = 0x8a8dcd000960; break;
  case 1736:
    l = 0x8a846e00095f; break;
  case 1737:
    l = 0x8a7b1000095e; break;
  case 1738:
    l = 0x8a71b400095c; break;
  case 1739:
    l = 0x8a685900095b; break;
  case 1740:
    l = 0x8a5eff00095a; break;
  case 1741:
    l = 0x8a55a6000959; break;
  case 1742:
    l = 0x8a4c4f000957; break;
  case 1743:
    l = 0x8a42f8000957; break;
  case 1744:
    l = 0x8a39a3000955; break;
  case 1745:
    l = 0x8a3050000953; break;
  case 1746:
    l = 0x8a26fd000953; break;
  case 1747:
    l = 0x8a1dac000951; break;
  case 1748:
    l = 0x8a145c000950; break;
  case 1749:
    l = 0x8a0b0e00094e; break;
  case 1750:
    l = 0x8a01c000094e; break;
  case 1751:
    l = 0x89f87400094c; break;
  case 1752:
    l = 0x89ef2900094b; break;
  case 1753:
    l = 0x89e5e0000949; break;
  case 1754:
    l = 0x89dc98000948; break;
  case 1755:
    l = 0x89d350000948; break;
  case 1756:
    l = 0x89ca0b000945; break;
  case 1757:
    l = 0x89c0c6000945; break;
  case 1758:
    l = 0x89b783000943; break;
  case 1759:
    l = 0x89ae40000943; break;
  case 1760:
    l = 0x89a500000940; break;
  case 1761:
    l = 0x899bc0000940; break;
  case 1762:
    l = 0x89928200093e; break;
  case 1763:
    l = 0x89894400093e; break;
  case 1764:
    l = 0x89800900093b; break;
  case 1765:
    l = 0x8976ce00093b; break;
  case 1766:
    l = 0x896d9400093a; break;
  case 1767:
    l = 0x89645c000938; break;
  case 1768:
    l = 0x895b25000937; break;
  case 1769:
    l = 0x8951f0000935; break;
  case 1770:
    l = 0x8948bb000935; break;
  case 1771:
    l = 0x893f88000933; break;
  case 1772:
    l = 0x893656000932; break;
  case 1773:
    l = 0x892d25000931; break;
  case 1774:
    l = 0x8923f5000930; break;
  case 1775:
    l = 0x891ac700092e; break;
  case 1776:
    l = 0x89119a00092d; break;
  case 1777:
    l = 0x89086e00092c; break;
  case 1778:
    l = 0x88ff4400092a; break;
  case 1779:
    l = 0x88f61a00092a; break;
  case 1780:
    l = 0x88ecf2000928; break;
  case 1781:
    l = 0x88e3cb000927; break;
  case 1782:
    l = 0x88daa5000926; break;
  case 1783:
    l = 0x88d181000924; break;
  case 1784:
    l = 0x88c85d000924; break;
  case 1785:
    l = 0x88bf3b000922; break;
  case 1786:
    l = 0x88b61a000921; break;
  case 1787:
    l = 0x88acfb00091f; break;
  case 1788:
    l = 0x88a3dc00091f; break;
  case 1789:
    l = 0x889abf00091d; break;
  case 1790:
    l = 0x8891a300091c; break;
  case 1791:
    l = 0x88888800091b; break;
  case 1792:
    l = 0x887f6f000919; break;
  case 1793:
    l = 0x887657000918; break;
  case 1794:
    l = 0x886d3f000918; break;
  case 1795:
    l = 0x886429000916; break;
  case 1796:
    l = 0x885b15000914; break;
  case 1797:
    l = 0x885201000914; break;
  case 1798:
    l = 0x8848ef000912; break;
  case 1799:
    l = 0x883fde000911; break;
  case 1800:
    l = 0x8836ce000910; break;
  case 1801:
    l = 0x882dbf00090f; break;
  case 1802:
    l = 0x8824b200090d; break;
  case 1803:
    l = 0x881ba600090c; break;
  case 1804:
    l = 0x88129a00090c; break;
  case 1805:
    l = 0x880991000909; break;
  case 1806:
    l = 0x880088000909; break;
  case 1807:
    l = 0x87f780000908; break;
  case 1808:
    l = 0x87ee7a000906; break;
  case 1809:
    l = 0x87e575000905; break;
  case 1810:
    l = 0x87dc71000904; break;
  case 1811:
    l = 0x87d36f000902; break;
  case 1812:
    l = 0x87ca6d000902; break;
  case 1813:
    l = 0x87c16d000900; break;
  case 1814:
    l = 0x87b86e0008ff; break;
  case 1815:
    l = 0x87af700008fe; break;
  case 1816:
    l = 0x87a6730008fd; break;
  case 1817:
    l = 0x879d780008fb; break;
  case 1818:
    l = 0x87947d0008fb; break;
  case 1819:
    l = 0x878b840008f9; break;
  case 1820:
    l = 0x87828c0008f8; break;
  case 1821:
    l = 0x8779950008f7; break;
  case 1822:
    l = 0x8770a00008f5; break;
  case 1823:
    l = 0x8767ab0008f5; break;
  case 1824:
    l = 0x875eb80008f3; break;
  case 1825:
    l = 0x8755c60008f2; break;
  case 1826:
    l = 0x874cd50008f1; break;
  case 1827:
    l = 0x8743e60008ef; break;
  case 1828:
    l = 0x873af70008ef; break;
  case 1829:
    l = 0x87320a0008ed; break;
  case 1830:
    l = 0x87291e0008ec; break;
  case 1831:
    l = 0x8720330008eb; break;
  case 1832:
    l = 0x8717490008ea; break;
  case 1833:
    l = 0x870e600008e9; break;
  case 1834:
    l = 0x8705790008e7; break;
  case 1835:
    l = 0x86fc930008e6; break;
  case 1836:
    l = 0x86f3ad0008e6; break;
  case 1837:
    l = 0x86eaca0008e3; break;
  case 1838:
    l = 0x86e1e70008e3; break;
  case 1839:
    l = 0x86d9050008e2; break;
  case 1840:
    l = 0x86d0250008e0; break;
  case 1841:
    l = 0x86c7460008df; break;
  case 1842:
    l = 0x86be670008df; break;
  case 1843:
    l = 0x86b58b0008dc; break;
  case 1844:
    l = 0x86acaf0008dc; break;
  case 1845:
    l = 0x86a3d40008db; break;
  case 1846:
    l = 0x869afb0008d9; break;
  case 1847:
    l = 0x8692230008d8; break;
  case 1848:
    l = 0x86894c0008d7; break;
  case 1849:
    l = 0x8680760008d6; break;
  case 1850:
    l = 0x8677a10008d5; break;
  case 1851:
    l = 0x866ecd0008d4; break;
  case 1852:
    l = 0x8665fb0008d2; break;
  case 1853:
    l = 0x865d2a0008d1; break;
  case 1854:
    l = 0x8654590008d1; break;
  case 1855:
    l = 0x864b8a0008cf; break;
  case 1856:
    l = 0x8642bd0008cd; break;
  case 1857:
    l = 0x8639f00008cd; break;
  case 1858:
    l = 0x8631240008cc; break;
  case 1859:
    l = 0x86285a0008ca; break;
  case 1860:
    l = 0x861f910008c9; break;
  case 1861:
    l = 0x8616c90008c8; break;
  case 1862:
    l = 0x860e020008c7; break;
  case 1863:
    l = 0x86053c0008c6; break;
  case 1864:
    l = 0x85fc780008c4; break;
  case 1865:
    l = 0x85f3b40008c4; break;
  case 1866:
    l = 0x85eaf20008c2; break;
  case 1867:
    l = 0x85e2310008c1; break;
  case 1868:
    l = 0x85d9710008c0; break;
  case 1869:
    l = 0x85d0b20008bf; break;
  case 1870:
    l = 0x85c7f40008be; break;
  case 1871:
    l = 0x85bf370008bd; break;
  case 1872:
    l = 0x85b67c0008bb; break;
  case 1873:
    l = 0x85adc20008ba; break;
  case 1874:
    l = 0x85a5080008ba; break;
  case 1875:
    l = 0x859c500008b8; break;
  case 1876:
    l = 0x8593990008b7; break;
  case 1877:
    l = 0x858ae40008b5; break;
  case 1878:
    l = 0x85822f0008b5; break;
  case 1879:
    l = 0x85797b0008b4; break;
  case 1880:
    l = 0x8570c90008b2; break;
  case 1881:
    l = 0x8568180008b1; break;
  case 1882:
    l = 0x855f680008b0; break;
  case 1883:
    l = 0x8556b90008af; break;
  case 1884:
    l = 0x854e0b0008ae; break;
  case 1885:
    l = 0x85455e0008ad; break;
  case 1886:
    l = 0x853cb30008ab; break;
  case 1887:
    l = 0x8534080008ab; break;
  case 1888:
    l = 0x852b5f0008a9; break;
  case 1889:
    l = 0x8522b70008a8; break;
  case 1890:
    l = 0x851a100008a7; break;
  case 1891:
    l = 0x85116a0008a6; break;
  case 1892:
    l = 0x8508c50008a5; break;
  case 1893:
    l = 0x8500210008a4; break;
  case 1894:
    l = 0x84f77f0008a2; break;
  case 1895:
    l = 0x84eedd0008a2; break;
  case 1896:
    l = 0x84e63d0008a0; break;
  case 1897:
    l = 0x84dd9e00089f; break;
  case 1898:
    l = 0x84d4ff00089f; break;
  case 1899:
    l = 0x84cc6200089d; break;
  case 1900:
    l = 0x84c3c700089b; break;
  case 1901:
    l = 0x84bb2c00089b; break;
  case 1902:
    l = 0x84b29200089a; break;
  case 1903:
    l = 0x84a9fa000898; break;
  case 1904:
    l = 0x84a162000898; break;
  case 1905:
    l = 0x8498cc000896; break;
  case 1906:
    l = 0x849037000895; break;
  case 1907:
    l = 0x8487a3000894; break;
  case 1908:
    l = 0x847f10000893; break;
  case 1909:
    l = 0x84767e000892; break;
  case 1910:
    l = 0x846ded000891; break;
  case 1911:
    l = 0x84655e00088f; break;
  case 1912:
    l = 0x845ccf00088f; break;
  case 1913:
    l = 0x84544200088d; break;
  case 1914:
    l = 0x844bb500088d; break;
  case 1915:
    l = 0x84432a00088b; break;
  case 1916:
    l = 0x843aa000088a; break;
  case 1917:
    l = 0x843217000889; break;
  case 1918:
    l = 0x84298f000888; break;
  case 1919:
    l = 0x842108000887; break;
  case 1920:
    l = 0x841882000886; break;
  case 1921:
    l = 0x840ffe000884; break;
  case 1922:
    l = 0x84077a000884; break;
  case 1923:
    l = 0x83fef8000882; break;
  case 1924:
    l = 0x83f677000881; break;
  case 1925:
    l = 0x83edf6000881; break;
  case 1926:
    l = 0x83e57700087f; break;
  case 1927:
    l = 0x83dcf900087e; break;
  case 1928:
    l = 0x83d47c00087d; break;
  case 1929:
    l = 0x83cc0000087c; break;
  case 1930:
    l = 0x83c38600087a; break;
  case 1931:
    l = 0x83bb0c00087a; break;
  case 1932:
    l = 0x83b293000879; break;
  case 1933:
    l = 0x83aa1c000877; break;
  case 1934:
    l = 0x83a1a6000876; break;
  case 1935:
    l = 0x839930000876; break;
  case 1936:
    l = 0x8390bc000874; break;
  case 1937:
    l = 0x838849000873; break;
  case 1938:
    l = 0x837fd7000872; break;
  case 1939:
    l = 0x837766000871; break;
  case 1940:
    l = 0x836ef6000870; break;
  case 1941:
    l = 0x83668700086f; break;
  case 1942:
    l = 0x835e1900086e; break;
  case 1943:
    l = 0x8355ad00086c; break;
  case 1944:
    l = 0x834d4100086c; break;
  case 1945:
    l = 0x8344d700086a; break;
  case 1946:
    l = 0x833c6d00086a; break;
  case 1947:
    l = 0x833405000868; break;
  case 1948:
    l = 0x832b9e000867; break;
  case 1949:
    l = 0x832338000866; break;
  case 1950:
    l = 0x831ad3000865; break;
  case 1951:
    l = 0x83126f000864; break;
  case 1952:
    l = 0x830a0c000863; break;
  case 1953:
    l = 0x8301aa000862; break;
  case 1954:
    l = 0x82f949000861; break;
  case 1955:
    l = 0x82f0e9000860; break;
  case 1956:
    l = 0x82e88b00085e; break;
  case 1957:
    l = 0x82e02d00085e; break;
  case 1958:
    l = 0x82d7d000085d; break;
  case 1959:
    l = 0x82cf7500085b; break;
  case 1960:
    l = 0x82c71b00085a; break;
  case 1961:
    l = 0x82bec100085a; break;
  case 1962:
    l = 0x82b669000858; break;
  case 1963:
    l = 0x82ae12000857; break;
  case 1964:
    l = 0x82a5bc000856; break;
  case 1965:
    l = 0x829d67000855; break;
  case 1966:
    l = 0x829513000854; break;
  case 1967:
    l = 0x828cc0000853; break;
  case 1968:
    l = 0x82846e000852; break;
  case 1969:
    l = 0x827c1d000851; break;
  case 1970:
    l = 0x8273cd000850; break;
  case 1971:
    l = 0x826b7f00084e; break;
  case 1972:
    l = 0x82633100084e; break;
  case 1973:
    l = 0x825ae400084d; break;
  case 1974:
    l = 0x82529900084b; break;
  case 1975:
    l = 0x824a4e00084b; break;
  case 1976:
    l = 0x824205000849; break;
  case 1977:
    l = 0x8239bd000848; break;
  case 1978:
    l = 0x823175000848; break;
  case 1979:
    l = 0x82292f000846; break;
  case 1980:
    l = 0x8220ea000845; break;
  case 1981:
    l = 0x8218a6000844; break;
  case 1982:
    l = 0x821062000844; break;
  case 1983:
    l = 0x820820000842; break;
  case 1984:
    l = 0x81ffdf000841; break;
  case 1985:
    l = 0x81f79f000840; break;
  case 1986:
    l = 0x81ef6100083e; break;
  case 1987:
    l = 0x81e72300083e; break;
  case 1988:
    l = 0x81dee600083d; break;
  case 1989:
    l = 0x81d6aa00083c; break;
  case 1990:
    l = 0x81ce6f00083b; break;
  case 1991:
    l = 0x81c636000839; break;
  case 1992:
    l = 0x81bdfd000839; break;
  case 1993:
    l = 0x81b5c5000838; break;
  case 1994:
    l = 0x81ad8f000836; break;
  case 1995:
    l = 0x81a559000836; break;
  case 1996:
    l = 0x819d25000834; break;
  case 1997:
    l = 0x8194f1000834; break;
  case 1998:
    l = 0x818cbf000832; break;
  case 1999:
    l = 0x81848e000831; break;
  case 2000:
    l = 0x817c5d000831; break;
  case 2001:
    l = 0x81742e00082f; break;
  case 2002:
    l = 0x816c0000082e; break;
  case 2003:
    l = 0x8163d200082e; break;
  case 2004:
    l = 0x815ba600082c; break;
  case 2005:
    l = 0x81537b00082b; break;
  case 2006:
    l = 0x814b5100082a; break;
  case 2007:
    l = 0x814328000829; break;
  case 2008:
    l = 0x813b00000828; break;
  case 2009:
    l = 0x8132d9000827; break;
  case 2010:
    l = 0x812ab3000826; break;
  case 2011:
    l = 0x81228e000825; break;
  case 2012:
    l = 0x811a6a000824; break;
  case 2013:
    l = 0x811247000823; break;
  case 2014:
    l = 0x810a25000822; break;
  case 2015:
    l = 0x810204000821; break;
  case 2016:
    l = 0x80f9e4000820; break;
  case 2017:
    l = 0x80f1c500081f; break;
  case 2018:
    l = 0x80e9a700081e; break;
  case 2019:
    l = 0x80e18b00081c; break;
  case 2020:
    l = 0x80d96f00081c; break;
  case 2021:
    l = 0x80d15400081b; break;
  case 2022:
    l = 0x80c93a00081a; break;
  case 2023:
    l = 0x80c122000818; break;
  case 2024:
    l = 0x80b90a000818; break;
  case 2025:
    l = 0x80b0f3000817; break;
  case 2026:
    l = 0x80a8de000815; break;
  case 2027:
    l = 0x80a0c9000815; break;
  case 2028:
    l = 0x8098b5000814; break;
  case 2029:
    l = 0x8090a3000812; break;
  case 2030:
    l = 0x808891000812; break;
  case 2031:
    l = 0x808080000811; break;
  case 2032:
    l = 0x80787100080f; break;
  case 2033:
    l = 0x80706200080f; break;
  case 2034:
    l = 0x80685500080d; break;
  case 2035:
    l = 0x80604800080d; break;
  case 2036:
    l = 0x80583d00080b; break;
  case 2037:
    l = 0x80503200080b; break;
  case 2038:
    l = 0x804829000809; break;
  case 2039:
    l = 0x804020000809; break;
  case 2040:
    l = 0x803818000808; break;
  case 2041:
    l = 0x803012000806; break;
  case 2042:
    l = 0x80280c000806; break;
  case 2043:
    l = 0x802008000804; break;
  case 2044:
    l = 0x801804000804; break;
  case 2045:
    l = 0x801002000802; break;
  case 2046:
    l = 0x800800000802; break;
  case 2047:
    l = 0x800000000800; break;
  default:
    l = 0xffffffffffff; break;  //ここは通過しないはず。
  }
  return (l);
}
