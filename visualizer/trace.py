import re
import numpy as np

class ThermalTrace:
    def __init__(self, temps: dict[str, dict[int, np.ndarray]]):
        self.temps = temps
        self._T = max(len(series) for comp in temps for series in temps[comp].values())

    @property
    def timesteps(self) -> int:
        return self._T

    def value(self, comp: str, idx: int, t: int, default: float = 0.0) -> float:
        series = self.temps.get(comp, {}).get(idx)
        if series is None or t >= len(series):
            return default
        return float(series[t])

    def global_minmax(self) -> tuple[float, float]:
        all_vals = np.concatenate([v for comp in self.temps for v in self.temps[comp].values()])
        return float(all_vals.min()), float(all_vals.max())

    @staticmethod
    def from_txt(path: str) -> "ThermalTrace":
        comp_re = re.compile(r'^\s*Component:\s*(\w+)\s*$')
        id_re   = re.compile(r'^\s*ID\s+(\d+)\s*:\s*(.*)$')

        temps: dict[str, dict[int, np.ndarray]] = {}
        cur = None

        with open(path, "r") as f:
            for raw in f:
                line = raw.strip()
                if not line:
                    continue

                m = comp_re.match(line)
                if m:
                    cur = m.group(1)
                    temps.setdefault(cur, {})
                    continue

                m = id_re.match(line)
                if m and cur is not None:
                    idx = int(m.group(1))
                    series = np.array([float(x) for x in m.group(2).split()], dtype=float)
                    temps[cur][idx] = series

        return ThermalTrace(temps)
