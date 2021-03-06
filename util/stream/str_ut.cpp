#include "str.h"

#include <library/unittest/registar.h>
#include <util/generic/typetraits.h>

template <typename T>
const T ReturnConstTemp();

Y_UNIT_TEST_SUITE(TStringInputTest) {
    Y_UNIT_TEST(Lvalue) {
        TString str = "Hello, World!";
        TStringInput input(str);

        TString result = input.ReadAll();

        UNIT_ASSERT_VALUES_EQUAL(result, str);
    }

    Y_UNIT_TEST(ConstRef) {
        TString str = "Hello, World!";
        const TString& r = str;
        TStringInput input(r);

        TString result = input.ReadAll();

        UNIT_ASSERT_VALUES_EQUAL(result, str);
    }

    Y_UNIT_TEST(NonConstRef) {
        TString str = "Hello, World!";
        TString& r = str;
        TStringInput input(r);

        TString result = input.ReadAll();

        UNIT_ASSERT_VALUES_EQUAL(result, str);
    }

    Y_UNIT_TEST(Transfer) {
        TString inputString = "some_string";
        TStringInput input(inputString);

        TString outputString;
        TStringOutput output(outputString);

        TransferData(&input, &output);

        UNIT_ASSERT_VALUES_EQUAL(inputString, outputString);
    }

    Y_UNIT_TEST(SkipReadAll) {
        TString string0 = "All animals are equal, but some animals are more equal than others.";

        TString string1;
        for (size_t i = 1; i <= string0.size(); i++)
            string1 += string0.substr(0, i);

        TStringInput input0(string1);

        size_t left = 5;
        while (left > 0)
            left -= input0.Skip(left);

        TString string2 = input0.ReadAll();

        UNIT_ASSERT_VALUES_EQUAL(string2, string1.substr(5));
    }

    Y_UNIT_TEST(OperatorBool) {
        TStringStream str;
        UNIT_ASSERT(!str);
        str << "data";
        UNIT_ASSERT(str);
        str.Clear();
        UNIT_ASSERT(!str);
    }

    Y_UNIT_TEST(TestReadTo) {
        TString s("0123456789abc");
        TString t;

        TStringInput in0(s);
        UNIT_ASSERT_VALUES_EQUAL(in0.ReadTo(t, '7'), 8);
        UNIT_ASSERT_VALUES_EQUAL(t, "0123456");
        UNIT_ASSERT_VALUES_EQUAL(in0.ReadTo(t, 'z'), 5);
        UNIT_ASSERT_VALUES_EQUAL(t, "89abc");
    }

    Y_UNIT_TEST(Write) {
        TString str;
        TStringOutput output(str);
        output << "1"
               << "22"
               << "333"
               << "4444"
               << "55555";

        UNIT_ASSERT_STRINGS_EQUAL(str, "1"
                                       "22"
                                       "333"
                                       "4444"
                                       "55555");
    }

    Y_UNIT_TEST(WriteChars) {
        TString str;
        TStringOutput output(str);
        output << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9' << '0';

        UNIT_ASSERT_STRINGS_EQUAL(str, "1234567890");
    }
}
