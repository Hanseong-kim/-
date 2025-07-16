class UlsanBusArrival:
    def __init__(self, stop_id, route_name, arrival_sec, remain_stops, present_stop_name):
        self.stop_id = stop_id
        self.route_name = route_name
        self.arrival_sec = int(arrival_sec)
        self.remain_stops = int(remain_stops)
        self.present_stop_name = present_stop_name

    @classmethod
    def from_dict(cls, d):
        return cls(
            stop_id=d.get("STOPID"),
            route_name=d.get("ROUTENM"),
            arrival_sec=d.get("ARRIVALTIME"),
            remain_stops=d.get("PREVSTOPCNT"),
            present_stop_name=d.get("PRESENTSTOPNM")
        )

    def dict(self):
        return {
            "stop_id": self.stop_id,
            "route_name": self.route_name,
            "arrival_time_sec": self.arrival_sec,
            "remain_stops": self.remain_stops,
            "present_stop_name": self.present_stop_name
        }
    def formatted_time(self):
        minutes = self.arrival_sec // 60
        seconds = self.arrival_sec % 60
        return f"{minutes}분 {seconds}초"

    def dict(self):
        return {
            "stop_id": self.stop_id,
            "route_name": self.route_name,
            "arrival_time_sec": self.arrival_sec,
            "arrival_time": self.formatted_time(),
            "remain_stops": self.remain_stops,
            "present_stop_name": self.present_stop_name
        }

class UlsanRoute:
    def __init__(self, route_id, name, type_):
        self.route_id = route_id
        self.name = name
        self.type = type_

    @classmethod
    def from_dict(cls, d):
        return cls(
            route_id=d.get("BRTID"),
            name=d.get("BRTNAME"),
            type_=d.get("BRTTYPE")
        )
