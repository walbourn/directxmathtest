//-------------------------------------------------------------------------------------
// triangle.cpp - DirectXMath Test Suite
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

#include "math3.h"

using namespace DirectX;

static const float EPSILON = 0.00001f;

inline bool IsEqual(const BoundingFrustum& f1, const BoundingFrustum& f2)
{
    return ((fabs(f1.Origin.x - f2.Origin.x) < EPSILON)
        && (fabs(f1.Origin.y - f2.Origin.y) < EPSILON)
        && (fabs(f1.Origin.z - f2.Origin.z) < EPSILON)
        && (fabs(f1.Orientation.x - f2.Orientation.x) < EPSILON)
        && (fabs(f1.Orientation.y - f2.Orientation.y) < EPSILON)
        && (fabs(f1.Orientation.z - f2.Orientation.z) < EPSILON)
        && (fabs(f1.Orientation.w - f2.Orientation.w) < EPSILON)
        && (fabs(f1.RightSlope - f2.RightSlope) < EPSILON)
        && (fabs(f1.LeftSlope - f2.LeftSlope) < EPSILON)
        && (fabs(f1.TopSlope - f2.TopSlope) < EPSILON)
        && (fabs(f1.BottomSlope - f2.BottomSlope) < EPSILON)
        && (fabs(f1.Near - f2.Near) < EPSILON)
        && (fabs(f1.Far - f2.Far) < EPSILON)) ? true : false;
}

#define printxmv(v) printe("%s: %f,%f,%f,%f\n", #v, XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v), XMVectorGetW(v))

#define printct(t) switch(t) { case DISJOINT: printe("%s: DISJOINT", #t); break; \
                               case INTERSECTS: printe("%s: INTERSECTS", #t); break; \
                               case CONTAINS: printe("%s: CONTAINS", #t); break; } printe("\n")

//-------------------------------------------------------------------------------------
// TriangleTests::Intersects
HRESULT TestT01(LogProxy* pLog)
{
    bool success = true;

    const XMVECTORF32 triangleA[3] =
    {
        { { { 0.5f, 0.5f, 0.5f, 0.f } } },
        { { { 1.0f, 0.f, 1.f, 0.f } } },
        { { { 01.f, 0.1f, 0.1f, 0.f } } }
    };

    const XMVECTORF32 triangleB[3] =
    {
        { { { 10.f, 10.f, 10.f, 0.f } } },
        { { { 2.0f, 0.f, 2.f, 0.f } } },
        { { { 5.f, 5.f, 5.f, 0.f } } }
    };

    // Triangle-Ray
    {
        float dist;

        XMVECTOR zero = XMVectorZero();

        const float check[3] = { 0.866025f, 1.414214f, 1.009951f };

        for (size_t t = 0; t < 3; ++t)
        {
            XMVECTOR dir = XMVectorSubtract(triangleA[t], zero);
            dir = XMVector3Normalize(dir);

            bool hit = TriangleTests::Intersects(zero, dir, triangleA[0], triangleA[1], triangleA[2], dist);

            if (!hit || (fabs(check[t] - dist) > EPSILON))
            {
                printe("%s: Triangle-Ray vertex test A%zu failed (hit=%s, dist=%f)\n", TestName, t, (hit) ? "Y" : "N", dist);
                printxmv(triangleA[0]);
                printxmv(triangleA[1]);
                printxmv(triangleA[2]);
                printxmv(zero);
                printxmv(dir);
                success = false;
            }

            dir = XMVectorSubtract(triangleB[t], zero);
            dir = XMVector3Normalize(dir);

            hit = TriangleTests::Intersects(zero, dir, triangleB[0], triangleB[1], triangleB[2], dist);
            if (hit) // Should miss
            {
                printe("%s: Triangle-Ray vertex test B%zu failed (hit=%s, dist=%f)\n", TestName, t, (hit) ? "Y" : "N", dist);
                printxmv(triangleB[0]);
                printxmv(triangleB[1]);
                printxmv(triangleB[2]);
                printxmv(zero);
                printxmv(dir);
                success = false;
            }
        }
    }

    {
        float dist;

        const XMVECTORF32 rayA[2] =
        {
            { { { 0.1f, 0.1f, 0.1f, 0.f } } }, { { { 1.f, 0.f, 0.f, 0.f } } }
        };
        bool hit = TriangleTests::Intersects(rayA[0], rayA[1], triangleA[0], triangleA[1], triangleA[2], dist);
        if (!hit || (fabs(dist - 0.9f) > EPSILON))
        {
            printe("%s: Triangle-Ray test A-A failed (hit=%s, dist=%f)\n", TestName, (hit) ? "Y" : "N", dist);
            printxmv(triangleA[0]);
            printxmv(triangleA[1]);
            printxmv(triangleA[2]);
            printxmv(rayA[0]);
            printxmv(rayA[1]);
            success = false;
        }

        hit = TriangleTests::Intersects(rayA[0], rayA[1], triangleB[0], triangleB[1], triangleB[2], dist);
        if (hit)
        {
            printe("%s: Triangle-Ray test B-A failed (hit=%s, dist=%f)\n", TestName, (hit) ? "Y" : "N", dist);
            printxmv(triangleB[0]);
            printxmv(triangleB[1]);
            printxmv(triangleB[2]);
            printxmv(rayA[0]);
            printxmv(rayA[1]);
            success = false;
        }

        const XMVECTORF32 rayB[2] =
        {
            { { { 10.f, 10.f, 10.f, 0.f } } }, { { { 0.f, 0.f, 1.f, 0.f } } }
        };
        hit = TriangleTests::Intersects(rayB[0], rayB[1], triangleA[0], triangleA[1], triangleA[2], dist);
        if (hit)
        {
            printe("%s: Triangle-Ray test A-B failed (hit=%s, dist=%f)\n", TestName, (hit) ? "Y" : "N", dist);
            printxmv(triangleA[0]);
            printxmv(triangleA[1]);
            printxmv(triangleA[2]);
            printxmv(rayB[0]);
            printxmv(rayB[1]);
            success = false;
        }

        hit = TriangleTests::Intersects(rayB[0], rayB[1], triangleB[0], triangleB[1], triangleB[2], dist);
        if (!hit || (fabs(dist) > EPSILON))
        {
            printe("%s: Triangle-Ray test B-B failed (hit=%s, dist=%f)\n", TestName, (hit) ? "Y" : "N", dist);
            printxmv(triangleB[0]);
            printxmv(triangleB[1]);
            printxmv(triangleB[2]);
            printxmv(rayB[0]);
            printxmv(rayB[1]);
            success = false;
        }

        const float sqrtOfOneThird = sqrtf(1.0f / 3.0f);
        const XMVECTORF32 rayC[2] =
        {
            { { { 10.f, 10.f, 10.f, 0.f } } },
            { { { -sqrtOfOneThird, -sqrtOfOneThird, -sqrtOfOneThird, 0.f } } }
        };
        const XMVECTORF32 triangleA_1[3] =
        {
            { { { 5.0f, 0.0f, 0.0f, 0.f } } },
            { { { 0.0f, 5.0f, 0.0f, 0.f } } },
            { { { 0.0f, 0.0f, 5.0f, 0.f } } }
        };
        hit = TriangleTests::Intersects(rayC[0], rayC[1], triangleA_1[0], triangleA_1[1], triangleA_1[2], dist);
        const float fExpectedDist = ((10.0f * sqrtOfOneThird) * 3.0f) - (5.0f * sqrtOfOneThird); //~14.433757f;
        if (!hit || (fabs(dist - fExpectedDist) > EPSILON))
        {
            printe("%s: Triangle-Ray test A-C failed (hit=%s, dist=%f)\n", TestName, (hit) ? "Y" : "N", dist);
            printxmv(triangleA_1[0]);
            printxmv(triangleA_1[1]);
            printxmv(triangleA_1[2]);
            printxmv(rayC[0]);
            printxmv(rayC[1]);
            success = false;
        }

        const XMVECTORF32 rayC_1[2] =
        {
            { { { 10.f, 10.f, 10.f, 0.f } } },
            { { { 0.0f, 0.0f, -1.0f, 0.f } } } // ray.Direction must be unit vector!
        };
        const XMVECTORF32 triangleB_1[3] =
        {
            { { { 5.f, 5.f, 10.f, 0.f } } },
            { { { 2.0f, 0.f, 2.f, 0.f } } },
            { { { 5.f, 5.f, 5.f, 0.f } } }
        };
        /*
         * edge = Triangle.P[2] - Triangle.P[0]
         * ray.Direction || edge
         * Cross product of two parallel vector is the zero vector
         *  ray.Direction x edge == ZeroVector
        */
        hit = TriangleTests::Intersects(rayC_1[0], rayC_1[1], triangleB_1[0], triangleB_1[1], triangleB_1[2], dist);
        if (hit)
        {
            printe("%s: Triangle-Ray test B-C failed (hit=%s, dist=%f)\n", TestName, (hit) ? "Y" : "N", dist);
            printxmv(triangleB_1[0]);
            printxmv(triangleB_1[1]);
            printxmv(triangleB_1[2]);
            printxmv(rayC_1[0]);
            printxmv(rayC_1[1]);
            success = false;
        }

        const XMVECTORF32 rayD[2] =
        {
            { { { -0.1f, -0.1f, -0.1f, 0.f } } }, { { { 0.f, 0.f, 1.f, 0.f } } }
        };
        hit = TriangleTests::Intersects(rayD[0], rayD[1], triangleA[0], triangleA[1], triangleA[2], dist);
        if (hit)
        {
            printe("%s: Triangle-Ray test A-D failed (hit=%s, dist=%f)\n", TestName, (hit) ? "Y" : "N", dist);
            printxmv(triangleA[0]);
            printxmv(triangleA[1]);
            printxmv(triangleA[2]);
            printxmv(rayD[0]);
            printxmv(rayD[1]);
            success = false;
        }

        hit = TriangleTests::Intersects(rayD[0], rayD[1], triangleB[0], triangleB[1], triangleB[2], dist);
        if (hit)
        {
            printe("%s: Triangle-Ray test B-D failed (hit=%s, dist=%f)\n", TestName, (hit) ? "Y" : "N", dist);
            printxmv(triangleB[0]);
            printxmv(triangleB[1]);
            printxmv(triangleB[2]);
            printxmv(rayD[0]);
            printxmv(rayD[1]);
            success = false;
        }
    }

    // Triangle-Triangle
    {
        if (!TriangleTests::Intersects(triangleA[0], triangleA[1], triangleA[2], triangleA[0], triangleA[1], triangleA[2]))
        {
            printe("%s: Triangle-Triangle test1 failed\n", TestName);
            printxmv(triangleA[0]);
            printxmv(triangleA[1]);
            printxmv(triangleA[2]);
            success = false;
        }

        if (TriangleTests::Intersects(triangleA[0], triangleA[1], triangleA[2], triangleB[0], triangleB[1], triangleB[2]))
        {
            printe("%s: Triangle-Triangle test3 failed\n", TestName);
            printxmv(triangleA[0]);
            printxmv(triangleA[1]);
            printxmv(triangleA[2]);
            printxmv(triangleB[0]);
            printxmv(triangleB[1]);
            printxmv(triangleB[2]);
            success = false;
        }
    }

    // Triangle-Plane
    {
        const XMVECTORF32 planes[9] =
        {
            { { { 0.f, 1.f, 0.f, 2.f } } },
            { { { 0.f, 1.f, 0.f, -2.f } } },
            { { { 0.f, 1.f, 0.f, 0.f } } },
            { { { 0.577350f, 0.577350f, 0.577350f, 2.f } } },
            { { { 0.577350f, 0.577350f, 0.577350f, -2.f } } },
            { { { 0.577350f, 0.577350f, 0.577350f, 0.f } } },
            { { { -0.577350f, -0.577350f, -0.577350f, 2.f } } },
            { { { -0.577350f, -0.577350f, -0.577350f, -2.f } } },
            { { { -0.577350f, -0.577350f, -0.577350f, 0.f } } }
        };

        PlaneIntersectionType resultA[9] =
        {
            FRONT,
            BACK,
            INTERSECTING,
            FRONT,
            BACK,
            FRONT,
            FRONT,
            BACK,
            BACK,
        };

        static_assert(std::size(planes) == std::size(resultA), "TestT01 Triangle-Plane testsA");

        PlaneIntersectionType resultB[9] =
        {
            FRONT,
            INTERSECTING,
            INTERSECTING,
            FRONT,
            FRONT,
            FRONT,
            BACK,
            BACK,
            BACK,
        };

        static_assert(std::size(planes) == std::size(resultB), "TestT01 Triangle-Plane testsB");

        for (size_t i = 0; i < std::size(planes); ++i)
        {
            PlaneIntersectionType p = TriangleTests::Intersects(triangleA[0], triangleA[1], triangleA[2], planes[i]);
            if (p != resultA[i])
            {
                printe("%s: Plane-Triangle testA failed ([%zu] result %d, expected %d)\n", TestName, i, p, resultA[i]);
                printxmv(triangleA[0]);
                printxmv(triangleA[1]);
                printxmv(triangleA[2]);
                printxmv(planes[i]);
                success = false;
            }

            p = TriangleTests::Intersects(triangleB[0], triangleB[1], triangleB[2], planes[i]);
            if (p != resultB[i])
            {
                printe("%s: Plane-Triangle testB failed ([%zu] result %d, expected %d)\n", TestName, i, p, resultB[i]);
                printxmv(triangleB[0]);
                printxmv(triangleB[1]);
                printxmv(triangleB[2]);
                printxmv(planes[i]);
                success = false;
            }
        }
    }

    return (success) ? S_OK : MATH_FAIL;
}


//-------------------------------------------------------------------------------------
// TriangleTests::ContainedBy
HRESULT TestT02(LogProxy* pLog)
{
    bool success = true;

    const XMVECTORF32 triangleA[3] =
    {
        { { { 0.5f, 0.5f, 0.5f, 0.f } } },
        { { { 1.0f, 0.f, 1.f, 0.f } } },
        { { { 01.f, 0.1f, 0.1f, 0.f } } }
    };

    const XMVECTORF32 triangleB[3] =
    {
        { { { 10.f, 10.f, 10.f, 0.f } } },
        { { { 2.0f, 0.f, 2.f, 0.f } } },
        { { { 5.f, 5.f, 5.f, 0.f } } }
    };

    {
        static const float test[][3] = { { 0, 0, -2.f }, { -2.0f, 2.0f,-2.0f }, {  0.0f, 2.0f,-2.0f }, {  2.0f, 2.0f,-2.0f },
                                         { -2.0f, 0.0f,-2.0f }, {  0.0f, 0.0f,-2.0f }, {  2.0f, 0.0f,-2.0f }, { -2.0f,-2.0f,-2.0f },
                                         {  0.0f,-2.0f,-2.0f }, {  2.0f,-2.0f,-2.0f } };
        for (size_t i = 0; i < sizeof(test) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test[i][0], test[i][1], test[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes(&Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5);

            ContainmentType ct = TriangleTests::ContainedBy(triangleA[0], triangleA[1], triangleA[2], Plane0, Plane1, Plane2, Plane3, Plane4, Plane5);
            if (ct != DISJOINT)
            {
                printe("%s: Failed triangle-6planes test1A %zu\n", TestName, i);
                printxmv(triangleA[0]);
                printxmv(triangleA[1]);
                printxmv(triangleA[2]);
                printxmv(Plane0);
                printxmv(Plane1);
                printxmv(Plane2);
                printxmv(Plane3);
                printxmv(Plane4);
                printxmv(Plane5);
                printct(ct);
                success = false;
            }

            ct = TriangleTests::ContainedBy(triangleB[0], triangleB[1], triangleB[2], Plane0, Plane1, Plane2, Plane3, Plane4, Plane5);
            if (ct != DISJOINT)
            {
                printe("%s: Failed triangle-6planes test1B %zu\n", TestName, i);
                printxmv(triangleB[0]);
                printxmv(triangleB[1]);
                printxmv(triangleB[2]);
                printxmv(Plane0);
                printxmv(Plane1);
                printxmv(Plane2);
                printxmv(Plane3);
                printxmv(Plane4);
                printxmv(Plane5);
                printct(ct);
                success = false;
            }
        }

        static const float test2[][3] = { { 2.f, 0, 0 }, { 0, 2.f, 0 }, { 0, -2.f, 0 },
                                         {  0.0f, 2.0f, 0.0f }, {  2.0f, 2.0f, 0.0f },
                                         {  0.0f,-2.0f, 0.0f }, {  2.0f,-2.0f, 0.0f }, {  2.0f, 0.0f, 0.0f }, };
        for (size_t i = 0; i < sizeof(test2) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test2[i][0], test2[i][1], test2[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes(&Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5);

            ContainmentType ct = TriangleTests::ContainedBy(triangleA[0], triangleA[1], triangleA[2], Plane0, Plane1, Plane2, Plane3, Plane4, Plane5);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed triangle-6planes test2A %zu\n", TestName, i);
                printxmv(triangleA[0]);
                printxmv(triangleA[1]);
                printxmv(triangleA[2]);
                printxmv(Plane0);
                printxmv(Plane1);
                printxmv(Plane2);
                printxmv(Plane3);
                printxmv(Plane4);
                printxmv(Plane5);
                printct(ct);
                success = false;
            }

            ct = TriangleTests::ContainedBy(triangleB[0], triangleB[1], triangleB[2], Plane0, Plane1, Plane2, Plane3, Plane4, Plane5);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed triangle-6planes test2B %zu\n", TestName, i);
                printxmv(triangleB[0]);
                printxmv(triangleB[1]);
                printxmv(triangleB[2]);
                printxmv(Plane0);
                printxmv(Plane1);
                printxmv(Plane2);
                printxmv(Plane3);
                printxmv(Plane4);
                printxmv(Plane5);
                printct(ct);
                success = false;
            }
        }

        static const float test3[][3] = { { 0, 0, 2.f }, { -2.0f, 2.0f, 2.0f }, { 0.0f, 2.0f, 2.0f }, {  2.0f, 2.0f, 2.0f },
                                          { -2.0f, 0.0f, 2.0f }, {  2.0f, 0.0f, 2.0f }, { -2.0f,-2.0f, 2.0f }, {  0.0f,-2.0f, 2.0f }, {  2.0f,-2.0f, 2.0f } };
        for (size_t i = 0; i < sizeof(test3) / (sizeof(float) * 3); ++i)
        {
            const XMFLOAT3 center(test3[i][0], test3[i][1], test3[i][2]);
            BoundingFrustum fr(center, XMFLOAT4(0.f, 0.f, 0.f, 1.f), 2.f, -2.f, 2.f, -2.f, 1.f, 2.f);

            XMVECTOR Plane0, Plane1, Plane2, Plane3, Plane4, Plane5;
            fr.GetPlanes(&Plane0, &Plane1, &Plane2, &Plane3, &Plane4, &Plane5);

            ContainmentType ct = TriangleTests::ContainedBy(triangleA[0], triangleA[1], triangleA[2], Plane0, Plane1, Plane2, Plane3, Plane4, Plane5);
            if (ct != DISJOINT)
            {
                printe("%s: Failed triangle-6planes test3A %zu\n", TestName, i);
                printxmv(triangleA[0]);
                printxmv(triangleA[1]);
                printxmv(triangleA[2]);
                printxmv(Plane0);
                printxmv(Plane1);
                printxmv(Plane2);
                printxmv(Plane3);
                printxmv(Plane4);
                printxmv(Plane5);
                printct(ct);
                success = false;
            }

            ct = TriangleTests::ContainedBy(triangleB[0], triangleB[1], triangleB[2], Plane0, Plane1, Plane2, Plane3, Plane4, Plane5);
            if (ct != INTERSECTS)
            {
                printe("%s: Failed triangle-6planes test3B %zu\n", TestName, i);
                printxmv(triangleB[0]);
                printxmv(triangleB[1]);
                printxmv(triangleB[2]);
                printxmv(Plane0);
                printxmv(Plane1);
                printxmv(Plane2);
                printxmv(Plane3);
                printxmv(Plane4);
                printxmv(Plane5);
                printct(ct);
                success = false;
            }
        }
    }

    return (success) ? S_OK : MATH_FAIL;
}
