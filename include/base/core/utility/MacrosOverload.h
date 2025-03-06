#pragma once 


#define _PP_CAT(a,b) a##b
#define PP_CAT(a,b) _PP_CAT(a,b)

#define BASE_MACRO_OVERLOAD_COMPAT_BLOCK(x) \
	do { \
		PP_CAT(x); \
	} while (0);

#define HUNDRED_COMMAS \
  , , , , , , , , , , \
  , , , , , , , , , , \
  , , , , , , , , , , \
  , , , , , , , , , , \
  , , , , , , , , , , \
  , , , , , , , , , , \
  , , , , , , , , , , \
  , , , , , , , , , , \
  , , , , , , , , , , \
  , , , , , , , , , ,


#define BASE_HUNDREDTH_ARGUMENT( \
    a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, \
    a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, \
    a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, \
    a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, \
    a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, \
    a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, \
    a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, \
    a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, \
    a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, \
    a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, ...) a100

#define BASE_COUNT_ARGS(...) \
    BASE_HUNDREDTH_ARGUMENT(_, ##__VA_ARGS__, \
        99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74, \
        73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48, \
        47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,  \
        21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)


#define BASE_MACRO_OVERLOAD_FUNC_CHOOSER( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, \
    a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, \
    a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, \
    a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, \
    a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, \
    a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, \
    a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, \
    a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, \
    a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, \
    a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, N, ... ) N

#define BASE_MACRO_OVERLOAD_FUNC_RECOMPOSER(argsWithParentheses)\
        BASE_MACRO_OVERLOAD_FUNC_CHOOSER argsWithParentheses

#define BASE_MACRO_OVERLOAD_MACRO_CHOOSER(target, ...)\
		BASE_MACRO_OVERLOAD_CHOOSE_FROM_ARG_COUNT(target, \
			target##BASE_MACRO_OVERLOAD_NO_ARG_EXPANDER(target))

#define BASE_MACRO_OVERLOAD_CHOOSE_FROM_ARG_COUNT(arg, ...) \
  BASE_MACRO_OVERLOAD_FUNC_RECOMPOSER((__VA_ARGS__, arg##_100, arg##_99, \
    arg##_98, arg##_97, arg##_96, \
    arg##_95, arg##_94, arg##_93, \
    arg##_92, arg##_91, arg##_90, \
    arg##_89, arg##_88, arg##_87, \
    arg##_86, arg##_85, arg##_84, \
    arg##_83, arg##_82, arg##_81, \
    arg##_80, arg##_79, arg##_78, \
    arg##_77, arg##_76, arg##_75, \
    arg##_74, arg##_73, arg##_72, \
    arg##_71, arg##_70, arg##_69, \
    arg##_68, arg##_67, arg##_66, \
    arg##_65, arg##_64, arg##_63, \
    arg##_62, arg##_61, arg##_60, \
    arg##_59, arg##_58, arg##_57, \
    arg##_56, arg##_55, arg##_54, \
    arg##_53, arg##_52, arg##_51, \
    arg##_50, arg##_49, arg##_48, \
    arg##_47, arg##_46, arg##_45, \
    arg##_44, arg##_43, arg##_42, \
    arg##_41, arg##_40, arg##_39, \
    arg##_38, arg##_37, arg##_36, \
    arg##_35, arg##_34, arg##_33, \
    arg##_32, arg##_31, arg##_30, \
    arg##_29, arg##_28, arg##_27, \
    arg##_26, arg##_25, arg##_24, \
    arg##_23, arg##_22, arg##_21, \
    arg##_20, arg##_19, arg##_18, \
    arg##_17, arg##_16, arg##_15, \
    arg##_14, arg##_13, arg##_12, \
    arg##_11, arg##_10, arg##_9, \
    arg##_8, arg##_7, arg##_6, \
    arg##_5, arg##_4, arg##_3, \
    arg##_2, arg##_1, ))


#define BASE_MACRO_OVERLOAD_NO_ARG_EXPANDER(target) \
        HUNDRED_COMMAS target##_0