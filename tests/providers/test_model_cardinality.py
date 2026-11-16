from pathlib import Path
import urllib.request,json,time

model=json.load(urllib.request.urlopen('http://127.0.0.1:8080/v1/models'))['data'][0]['id']
schema={'type':'object','properties':{'assessments':{'type':'array','minItems':185,'maxItems':185,'items':{'type':'string','enum':['covered']}}},'required':['assessments'],'additionalProperties':False}
body={'model':model,'stream':False,'messages':[{'role':'user','content':'Return a JSON object with an assessments array containing 1000 copies of the string covered.'}],'chat_template_kwargs':{'enable_thinking':False},'echo_background':False,'response_format':{'type':'json_schema','json_schema':{'name':'exact_assessment_cardinality','strict':True,'schema':schema}}}
start=time.monotonic();request=urllib.request.Request('http://127.0.0.1:8080/v1/chat/completions',data=json.dumps(body).encode(),headers={'Content-Type':'application/json'});response=json.load(urllib.request.urlopen(request));parsed=json.loads(response['choices'][0]['message']['content']);assert len(parsed['assessments'])==185
receipt={'requested_by_prompt':1000,'required_by_contract':185,'actual_assessments':len(parsed['assessments']),'seconds':time.monotonic()-start,'passed':True};print(json.dumps(receipt))
