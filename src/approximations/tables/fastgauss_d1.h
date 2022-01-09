const float offset = 65536.f, limit = 4.f;
static const float fastgauss_lut[] = {1.00001716f, -0.0117180459f, 1.00035073f, -0.0390458147f, 1.00120316f, -0.0702224382f, 1.00253826f, -0.101296305f, 1.00435018f, -0.132222079f, 1.00663094f, -0.162954783f, 1.00937053f, -0.193449909f, 1.01255688f, -0.223663529f, 1.01617595f, -0.253552394f, 1.0202118f, -0.283074045f, 1.02464661f, -0.312186905f, 1.0294608f, -0.340850384f, 1.03463306f, -0.369024967f, 1.04014048f, -0.396672309f, 1.04595863f, -0.42375532f, 1.05206161f, -0.450238248f, 1.05842224f, -0.476086753f, 1.06501208f, -0.501267989f, 1.07180162f, -0.525750662f, 1.07876032f, -0.549505103f, 1.0858568f, -0.572503319f, 1.09305891f, -0.59471905f, 1.10033388f, -0.616127812f, 1.10764843f, -0.636706944f, 1.11496893f, -0.656435639f, 1.12226147f, -0.675294977f, 1.12949206f, -0.693267948f, 1.13662668f, -0.710339469f, 1.14363148f, -0.726496399f, 1.15047283f, -0.741727544f, 1.15711753f, -0.75602366f, 1.16353284f, -0.769377444f, 1.16968664f, -0.781783528f, 1.17554757f, -0.793238461f, 1.18108508f, -0.80374069f, 1.18626957f, -0.81329053f, 1.19107249f, -0.821890136f, 1.1954664f, -0.829543465f, 1.19942513f, -0.836256235f, 1.20292377f, -0.842035884f, 1.20593882f, -0.846891513f, 1.20844824f, -0.850833841f, 1.21043148f, -0.853875142f, 1.2118696f, -0.856029187f, 1.21274526f, -0.857311181f, 1.2130428f, -0.857737696f, 1.21274826f, -0.857326602f, 1.21184941f, -0.856096997f, 1.21033579f, -0.85406913f, 1.2081987f, -0.851264333f, 1.20543122f, -0.847704936f, 1.20202822f, -0.843414199f, 1.19798632f, -0.838416225f, 1.19330393f, -0.832735887f, 1.1879812f, -0.826398747f, 1.18201999f, -0.819430975f, 1.17542385f, -0.811859275f, 1.16819798f, -0.803710801f, 1.16034922f, -0.795013085f, 1.15188593f, -0.785793954f, 1.14281802f, -0.776081461f, 1.13315683f, -0.76590381f, 1.12291513f, -0.755289278f, 1.11210698f, -0.744266154f, 1.10074773f, -0.732862663f, 1.08885393f, -0.721106903f, 1.07644325f, -0.709026784f, 1.06353438f, -0.696649962f, 1.050147f, -0.684003783f, 1.03630169f, -0.671115228f, 1.02201981f, -0.658010859f, 1.00732346f, -0.644716768f, 0.992235397f, -0.631258533f, 0.976778919f, -0.617661172f, 0.960977819f, -0.603949103f, 0.944856286f, -0.590146107f, 0.928438828f, -0.57627529f, 0.911750193f, -0.562359058f, 0.89481529f, -0.548419082f, 0.877659114f, -0.53447628f, 0.860306668f, -0.520550789f, 0.842782895f, -0.506661948f, 0.825112603f, -0.492828284f, 0.807320405f, -0.479067499f, 0.789430644f, -0.465396457f, 0.771467341f, -0.451831179f, 0.753454129f, -0.438386838f, 0.735414202f, -0.425077758f, 0.717370261f, -0.411917413f, 0.699344465f, -0.39891843f, 0.681358385f, -0.386092598f, 0.663432964f, -0.37345087f, 0.645588476f, -0.361003376f, 0.627844494f, -0.348759433f, 0.610219857f, -0.336727561f, 0.592732643f, -0.324915494f, 0.575400144f, -0.3133302f, 0.558238847f, -0.301977897f, 0.541264415f, -0.290864074f, 0.524491674f, -0.27999351f, 0.507934604f, -0.269370297f, 0.491606328f, -0.258997862f, 0.47551911f, -0.248878991f, 0.459684354f, -0.23901585f, 0.444112605f, -0.229410015f, 0.428813554f, -0.220062495f, 0.413796043f, -0.210973755f, 0.399068074f, -0.202143746f, 0.384636826f, -0.193571929f, 0.370508662f, -0.1852573f, 0.356689148f, -0.17719842f, 0.343183072f, -0.169393435f, 0.329994461f, -0.161840109f, 0.317126603f, -0.154535845f, 0.30458207f, -0.14747771f, 0.292362739f, -0.140662462f, 0.280469823f, -0.134086574f, 0.268903888f, -0.127746257f, 0.257664889f, -0.121637485f, 0.246752189f, -0.115756016f, 0.236164593f, -0.110097413f, 0.225900371f, -0.10465707f, 0.215957294f, -0.0994302284f, 0.206332653f, -0.0944119983f, 0.197023297f, -0.0895973781f, 0.188025656f, -0.0849812722f, 0.179335773f, -0.080558509f, 0.170949329f, -0.0763238573f, 0.162861673f, -0.0722720427f, 0.155067853f, -0.0683977621f, 0.147562635f, -0.0646956985f, 0.140340538f, -0.0611605339f, 0.133395854f, -0.0577869625f, 0.126722678f, -0.0545697021f, 0.120314926f, -0.0515035053f, 0.114166367f, -0.0485831694f, 0.108270638f, -0.0458035464f, 0.102621273f, -0.0431595513f, 0.0972117167f, -0.0406461701f, 0.0920353515f, -0.0382584671f, 0.0870855124f, -0.0359915915f, 0.082355507f, -0.0338407832f, 0.0778386323f, -0.0318013781f, 0.0735281909f, -0.0298688128f, 0.0694175069f, -0.0280386285f, 0.0654999396f, -0.0263064744f, 0.0617688974f, -0.024668111f, 0.0582178501f, -0.0231194119f, 0.0548403404f, -0.0216563663f, 0.0516299946f, -0.0202750803f, 0.0485805323f, -0.0189717778f, 0.0456857753f, -0.0177428012f, 0.0429396556f, -0.0165846117f, 0.0403362224f, -0.0154937891f, 0.0378696487f, -0.0144670316f, 0.035534237f, -0.0135011548f, 0.0333244237f, -0.0125930907f, 0.0312347839f, -0.0117398867f, 0.0292600346f, -0.0109387038f, 0.0273950375f, -0.0101868152f, 0.0256348013f, -0.0094816041f, 0.0239744837f, -0.00882056172f, 0.0224093921f, -0.0082012852f, 0.0209349845f, -0.00762147513f, 0.0195468694f, -0.00707893315f, 0.0182408061f, -0.00657155937f, 0.0170127034f, -0.00609734972f, 0.0158586189f, -0.00565439326f, 0.0147747573f, -0.0052408694f, 0.0137574691f, -0.00485504513f, 0.0128032481f, -0.00449527218f, 0.0119087295f, -0.00415998417f, 0.0110706871f, -0.00384769381f, 0.0102860309f, -0.00355698997f, 0.00955180374f, -0.00328653496f, 0.00886517868f, -0.00303506165f, 0.00822345549f, -0.00280137073f, 0.00762405745f, -0.00258432794f, 0.00706452791f, -0.00238286141f, 0.00654252674f, -0.00219595899f, 0.00605582685f, -0.00202266565f, 0.00560231047f, -0.00186208094f, 0.00517996564f, -0.0017133565f, 0.00478688244f, -0.00157569368f, 0.00442124944f, -0.00144834113f, 0.00408134998f, -0.00133059256f, 0.0037655586f, -0.0012217845f, 0.00347233746f, -0.00112129417f, 0.00320023276f, -0.00102853737f, 0.00294787128f, -0.000942966532f, 0.00271395696f, -0.000864068766f, 0.0024972675f, -0.000791363989f, 0.00229665108f, -0.00072440316f, 0.00211102314f, -0.000662766558f, 0.00193936324f, -0.000606062142f, 0.00178071201f, -0.000553923978f, 0.00163416817f, -0.000506010734f, 0.00149888565f, -0.000462004246f, 0.00137407083f, -0.000421608143f, 0.0012589798f, -0.000384546544f, 0.00115291579f, -0.000350562812f, 0.00105522666f, -0.000319418373f, 0.000965302483f, -0.000290891591f, 0.000882573223f, -0.000264776705f, 0.000806506513f, -0.000240882814f, 0.000736605523f, -0.000219032922f, 0.000672406916f, -0.000199063033f, 0.0006134789f, -0.000180821295f, 0.00055941936f, -0.000164167192f, 0.000509854083f, -0.000148970783f, 0.000464435066f, -0.000135111981f, 0.000422838902f, -0.000122479878f, 0.00038476525f, -0.000110972111f, 0.000349935384f, -0.00010049426f, 0.000318090809f, -9.09592907e-05f, 0.000288991963f, -8.22870248e-05f, 0.000262416975f, -7.44036482e-05f, 0.000238160499f, -6.72412476e-05f, 0.000216032612f, -6.07373783e-05f, 0.000195857771f, -5.48346596e-05f, 0.000177473834f, -4.9480397e-05f, 0.000160731137f, -4.46262296e-05f, 0.000145491623f, -4.02278009e-05f, 0.000131628026f, -3.62444525e-05f, 0.000119023105f, -3.26389389e-05f, 0.000107568927f, -2.93771615e-05f, 9.71661888e-05f, -2.64279222e-05f, 8.77235882e-05f, -2.37626939e-05f, 7.91572342e-05f, -2.13554076e-05f, 7.13900943e-05f, -1.91822554e-05f, 6.43514793e-05f, -1.72215073e-05f, 5.79765624e-05f, -1.54533416e-05f, 5.22059313e-05f, -1.38596888e-05f, 4.698517e-05f, -1.2424086e-05f, 4.22644709e-05f, -1.11315435e-05f, 3.7998273e-05f, -9.96842067e-06f, 3.41449258e-05f, -8.92231247e-06f, 3.06663783e-05f, -7.98194385e-06f, 2.7527889e-05f, -7.13707302e-06f, 2.46977583e-05f, -6.37840233e-06f, 2.214708e-05f, -5.69749616e-06f, 1.98495115e-05f, -5.08670552e-06f, 1.77810616e-05f, -4.53909883e-06f, 1.59198935e-05f, -4.04839825e-06f, 1.42461444e-05f, -3.60892144e-06f, 1.27417574e-05f, -3.215528e-06f, 1.13903278e-05f, -2.86357058e-06f, 1.01769611e-05f, -2.54884998e-06f, 9.08814184e-06f, -2.26757417e-06f, 8.11161377e-06f, -2.0163208e-06f, 7.2362691e-06f, -1.79200293e-06f, 6.452047e-06f, -1.59183781e-06f, 5.74984047e-06f, -1.41331831e-06f, 5.12141084e-06f, -1.25418695e-06f, 4.5593095e-06f, -1.11241221e-06f, 4.05680615e-06f, -9.86166958e-07f, 3.76866451e-06f, -9.14034018e-07f};