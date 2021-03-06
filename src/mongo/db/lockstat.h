// lockstat.h

/**
*    Copyright (C) 2008 10gen Inc.
*
*    This program is free software: you can redistribute it and/or  modify
*    it under the terms of the GNU Affero General Public License, version 3,
*    as published by the Free Software Foundation.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*    As a special exception, the copyright holders give permission to link the
*    code of portions of this program with the OpenSSL library under certain
*    conditions as described in each individual source file and distribute
*    linked combinations including the program with the OpenSSL library. You
*    must comply with the GNU Affero General Public License in all respects for
*    all of the code used other than as permitted herein. If you modify file(s)
*    with this exception, you may extend this exception to your version of the
*    file(s), but you are not obligated to do so. If you do not wish to do so,
*    delete this exception statement from your version. If you delete this
*    exception statement from all source files in the program, then also delete
*    it in the license file.
*/


#pragma once

#include "mongo/platform/atomic_word.h"
#include "mongo/util/timer.h"

namespace mongo { 

    class BSONObj;

    class LockStat { 
        enum { N = 4 };
    public:
        void recordAcquireTimeMicros( char type , long long micros );
        void recordLockTimeMicros( char type , long long micros );

        void reset();

        BSONObj report() const;
        void report( StringBuilder& builder ) const;

        long long getTimeLocked( char type ) const { return timeLocked[mapNo(type)].load(); }
    private:
        static void _append( BSONObjBuilder& builder,
                             const AtomicInt64* data,
                             const AtomicInt64* additionalIndicator );
        
        // RWrw
        // in micros
        AtomicInt64 timeAcquiring[N];
        AtomicInt64 timeLocked[N];

        static unsigned mapNo(char type);
        static char nameFor(unsigned offset);
    };

}
