library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity Clock_Divider is
    port (
        clk_in : in std_logic;
        clk_out : out std_logic
    );
end Clock_Divider;

architecture Behavioral of Clock_Divider is
    signal counter : unsigned(25 downto 0) := (others => '0');
begin
    process(clk_in)
    begin
        if rising_edge(clk_in) then
            counter <= counter + 1;
            if counter = 50000000 then -- Adjust the value for desired blink rate
                counter <= (others => '0');
                clk_out <= not clk_out;
            end if;
        end if;
    end process;
end Behavioral;
