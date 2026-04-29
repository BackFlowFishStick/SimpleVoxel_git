//
// Created by Danmeng on 2026/3/24.
//

#ifndef SIMPLEVOXEL_TEST_H
#define SIMPLEVOXEL_TEST_H

#pragma once

namespace test
{
    class Test
    {
    public:
        Test() {}
        virtual ~Test() {}

        virtual void onUpdate(float deltaTime) {}

        virtual void onRender() {}

        virtual void onImGuiRender() {}


    };

}

#endif //SIMPLEVOXEL_TEST_H