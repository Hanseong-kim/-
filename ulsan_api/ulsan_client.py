import requests
from utils import parse_xml
from models import UlsanBusArrival, UlsanRoute

API_KEY = "of2uRvSbvTiVNqne5ADOOtlEX/QJEs9J1iZo4WLl5agdN+jL7ude42RZTvCk4X6NKwuk3J5wyM/WaxZrJTMgxg=="
BASE_URL = "http://openapi.its.ulsan.kr/UlsanAPI"

def get_arrival_by_stop(stopid: str, page: int = 1, rows: int = 10):
    url = f"{BASE_URL}/getBusArrivalInfo.xo"
    params = {
        "serviceKey": API_KEY,
        "stopid": stopid,
        "pageNo": page,
        "numOfRows": rows
    }
    xml = requests.get(url, params=params).content
    items = parse_xml(xml, "row")
    return [UlsanBusArrival.from_dict(item) for item in items]

def get_routes(page: int = 1, rows: int = 100):
    url = f"{BASE_URL}/RouteInfo.xo"
    params = {
        "serviceKey": API_KEY,
        "pageNo": page,
        "numOfRows": rows
    }
    xml = requests.get(url, params=params).content
    items = parse_xml(xml, "row")
    return [UlsanRoute.from_dict(item) for item in items]


def search_stop_by_name(name: str, page: int = 1, rows: int = 1000):
    url = f"{BASE_URL}/BusStopInfo.xo"
    params = {
        "serviceKey": API_KEY,
        "pageNo": page,
        "numOfRows": rows
    }
    xml = requests.get(url, params=params).content
    items = parse_xml(xml, "row")
    name = name.strip().lower()
    return [item for item in items if name in item.get("STOPNAME", "").strip().lower()]


