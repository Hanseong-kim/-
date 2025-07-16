from flask import Flask, request, jsonify
from ulsan_client import get_arrival_by_stop
from ulsan_client import search_stop_by_name
from flask import render_template

app = Flask(__name__)

@app.route("/ulsan/arrival")
def arrival():
    stopid = request.args.get("stopid")
    if not stopid:
        return jsonify({"error": "stopid is required"}), 400
    arrivals = get_arrival_by_stop(stopid)
    return jsonify([a.dict() for a in arrivals])

@app.route("/ulsan/arrival/view")
def arrival_html():
    stopid = request.args.get("stopid")
    if not stopid:
        return "stopid query param is required", 400
    arrivals = get_arrival_by_stop(stopid)
    return render_template("arrival.html", stop_id=stopid, arrivals=arrivals)

@app.route("/ulsan/lines_at_stop")
def lines_at_stop():
    stopid = request.args.get("stopid")
    if not stopid:
        return jsonify({"error": "stopid is required"}), 400
    arrivals = get_arrival_by_stop(stopid)
    lines = list(set([a.route_name for a in arrivals]))
    return jsonify(sorted(lines))

@app.route("/ulsan/search_stop")
def search_stop():
    name = request.args.get("name")
    if not name:
        return "검색어(name)가 필요합니다.", 400
    results = search_stop_by_name(name)
    return render_template("stop_list.html", name=name, results=results)


if __name__ == "__main__":
    app.run(debug=True)
