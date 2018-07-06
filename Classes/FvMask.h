//
// Created by farmer on 2018/7/6.
// 标记位工具
//

#ifndef COCOSTUDIO_MAHJONG_FVMASK_H
#define COCOSTUDIO_MAHJONG_FVMASK_H

namespace FvMask {
    template<typename TFlag, typename TMask>
    bool HasAny(const TFlag flag, const TMask mask) {
        return ((flag & mask) != 0);
    }

    template<typename TFlag, typename TMask>
    bool HasAll(const TFlag flag, const TMask mask) {
        return ((flag & mask) == mask);
    }

    template<typename TFlag, typename TMask>
    void Add(TFlag &flag, const TMask mask) {
        flag |= mask;
    }

    template<typename TFlag, typename TMask>
    void Del(TFlag &flag, const TMask mask) {
        flag &= ~mask;
    }

    template<typename TFlag, typename TMask>
    TFlag Remove(const TFlag flag, const TMask mask) {
        return (flag & (~mask));
    }

    template<typename TFlag, typename TMask>
    TFlag IsAdd(const TFlag oldFlag, const TFlag newFlag, const TMask mask) {
        return (((oldFlag & mask) == 0) && ((newFlag & mask) != 0));
    }

    template<typename TFlag, typename TMask>
    TFlag IsDel(const TFlag oldFlag, const TFlag newFlag, const TMask mask) {
        return (((oldFlag & mask) != 0) && ((newFlag & mask) == 0));
    }
}
#ifndef _MASK_
#define _MASK_(flag) (0X01 << (flag))
#endif


#endif //COCOSTUDIO_MAHJONG_FVMASK_H
