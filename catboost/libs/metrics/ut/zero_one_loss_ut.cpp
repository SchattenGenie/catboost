#include <library/unittest/registar.h>
#include <catboost/libs/metrics/metric.h>
#include <catboost/libs/metrics/metric_holder.h>

//The benchmark value was calculated by sklearn.metrics.zero_one_loss
Y_UNIT_TEST_SUITE(ZeroOneLossMetricTest) {
Y_UNIT_TEST(ZeroOneLossTest) {
    {
        TVector<TVector<double>> approx{{0, 1, 1, 0}};
        TVector<float> target{0, 1, 1, 0};
        TVector<float> weight{1, 1, 1, 1};

        TZeroOneLossMetric metric;
        TMetricHolder score = metric.EvalSingleThread(approx, target, weight, {}, 0, 4);

        UNIT_ASSERT_DOUBLES_EQUAL(metric.GetFinalError(score), 0, 1e-1);
    }
    {
        TVector<TVector<double>> approx{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        TVector<float> target{0, 0, 2, 3};
        TVector<float> weight{1, 1, 1, 1};

        TZeroOneLossMetric metric;
        TMetricHolder score = metric.EvalSingleThread(approx, target, weight, {}, 0, target.size());

        UNIT_ASSERT_DOUBLES_EQUAL(metric.GetFinalError(score), 0.25, 1e-1);
    }
    {
        TVector<TVector<double>> approx{{1, 1, 1, 1}, {0, 0, 0, 0}};
        TVector<float> target{0, 0, 0, 0};
        TVector<float> weight{0.26705f, 0.666578f, 0.6702279f, 0.3976618f};

        TZeroOneLossMetric metric;
        TMetricHolder score = metric.EvalSingleThread(approx, target, weight, {}, 0, target.size());

        UNIT_ASSERT_DOUBLES_EQUAL(metric.GetFinalError(score), 0, 1e-1);
    }
    {
        TVector<TVector<double>> approx{{1, 0, 0, 1}};
        TVector<float> target{0, 1, 1, 0};
        TVector<float> weight{1, 1, 1, 1};

        TZeroOneLossMetric metric;
        TMetricHolder score = metric.EvalSingleThread(approx, target, weight, {}, 0, 4);

        UNIT_ASSERT_DOUBLES_EQUAL(metric.GetFinalError(score), 1, 1e-1);
    }
    {
        TVector<TVector<double>> approx{{1, 1, 0, 0}};
        TVector<float> target{0, 1, 1, 0};
        TVector<float> weight{1, 1, 1, 1};

        TZeroOneLossMetric metric;
        TMetricHolder score = metric.EvalSingleThread(approx, target, weight, {}, 0, 4);

        UNIT_ASSERT_DOUBLES_EQUAL(metric.GetFinalError(score), 0.5, 1e-1);
    }
}
}
