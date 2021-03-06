-------------------------------------------------------------------------------
-- Project    : Welec W2000A 
-------------------------------------------------------------------------------
-- File       : Devices-p.vhd
-- Author     : Alexander Lindert <alexander_lindert at gmx.at>
-- Created    : 2009-02-14
-- Last update: 2009-02-14
-- Platform   : 
-------------------------------------------------------------------------------
-- Description: Out of date
-------------------------------------------------------------------------------
--  Copyright (c) 2008, Alexander Lindert
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
--
--  For commercial applications where source-code distribution is not
--  desirable or possible, I offer low-cost commercial IP licenses.
--  Please contact me per mail.
-------------------------------------------------------------------------------
-- Revisions  :
-- Date        Version 
-- 2009-02-14  1.0      
-------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use work.Global.all;


package pDevices is

  constant cMemAddrRange     : natural := 0;
  constant cTriggerAddrRange : natural := 1;
  constant cSFRAddrRange     : natural := 2;
  constant cBootAddrRange    : natural := 3;
  constant cSlaves           : natural := 4;
  constant cSelectBits       : natural := 2;  -- ceil(Log2(cSlaves))
  constant cBootDataSize     : natural := 9;  -- Log2(LastBootData from boot Rom)

  type aBus is array (natural range<>) of aDword;
end;
