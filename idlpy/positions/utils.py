from typing import Union
from .._idlpy import Position
from typing import Union

def equal_position(lhs,
                   rhs) -> bool:
    for it_lhs, it_rhs in zip(lhs.hedges, rhs.hedges):
        if it_lhs != it_rhs:
            return False

    return lhs.jtd    == rhs.jtd and \
        lhs.notional  == rhs.notional and \
        lhs.sector    == rhs.sector and \
        lhs.region    == rhs.region and \
        lhs.rating    == rhs.rating and \
        lhs.hedges    == rhs.hedges

